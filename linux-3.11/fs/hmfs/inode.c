#include "hmfs_fs.h"
#include "hmfs.h"

struct backing_dev_info hmfs_backing_dev_info __read_mostly = {
	.ra_pages = 0,
	.capabilities = BDI_CAP_NO_ACCT_AND_WRITEBACK,
};

void hmfs_set_inode_flags(struct inode *inode)
{
	unsigned int flags = HMFS_I(inode)->i_flags;

	inode->i_flags &= ~(S_SYNC | S_APPEND | S_IMMUTABLE | S_NOATIME | 
			S_DIRSYNC);

	if (flags & FS_SYNC_FL)
		inode->i_flags |= S_SYNC;
	if (flags & FS_APPEND_FL)
		inode->i_flags |= S_APPEND;
	if (flags & FS_IMMUTABLE_FL)
		inode->i_flags |= S_IMMUTABLE;
	if (flags & FS_NOATIME_FL)
		inode->i_flags |= S_NOATIME;
	if (flags & FS_DIRSYNC_FL)
		inode->i_flags |= S_DIRSYNC;
}

int hmfs_convert_inline_inode(struct inode *inode)
{
	struct hmfs_inode *old_inode_block, *new_inode_block;
	struct hmfs_sb_info *sbi = HMFS_I_SB(inode);
	umode_t mode = inode->i_mode;
	size_t i_size;
	void *data_block;

	if (S_ISDIR(mode)) {
		i_size = HMFS_INLINE_SIZE;
	} else {
		i_size = i_size_read(inode);
	}

	hmfs_bug_on(sbi, !is_inline_inode(inode));
	old_inode_block = get_node(sbi, inode->i_ino);
	if (IS_ERR(old_inode_block))
		return PTR_ERR(old_inode_block);
	
	set_inode_flag(HMFS_I(inode), FI_CONVERT_INLINE);
	new_inode_block = alloc_new_node(sbi, inode->i_ino, inode, SUM_TYPE_INODE, false);
	clear_inode_flag(HMFS_I(inode), FI_CONVERT_INLINE);
	if (IS_ERR(new_inode_block))
		return PTR_ERR(new_inode_block);

	hmfs_bug_on(sbi, old_inode_block == new_inode_block);

	/* Reinitialize i_addrs */
	memset(new_inode_block->i_addr, 0, sizeof(__le64) * 
			NORMAL_ADDRS_PER_INODE + sizeof(__le32) * 5);
	data_block = alloc_new_data_block(sbi, inode, 0);
	hmfs_memcpy(data_block, old_inode_block->inline_content, 
					i_size);
	if (S_ISDIR(mode))
		mark_size_dirty(inode, HMFS_BLOCK_SIZE[SEG_NODE_INDEX]);

	clear_inode_flag(HMFS_I(inode), FI_INLINE_DATA);
	return 0;
}

static int do_read_inode(struct inode *inode)
{
	struct hmfs_sb_info *sbi = HMFS_I_SB(inode);
	struct hmfs_inode_info *fi = HMFS_I(inode);
	struct hmfs_node *hn;
	struct hmfs_inode *hi;

	if (check_nid_range(sbi, inode->i_ino)) {
		return -EINVAL;
	}

	hn = (struct hmfs_node *)get_node(sbi, inode->i_ino);
	if (IS_ERR(hn))
		return PTR_ERR(hn);
	hi = &hn->i;

	inode->i_mode = le16_to_cpu(hi->i_mode);
	i_uid_write(inode, le32_to_cpu(hi->i_uid));
	i_gid_write(inode, le32_to_cpu(hi->i_gid));
	set_nlink(inode, le32_to_cpu(hi->i_links));
	inode->i_size = le64_to_cpu(hi->i_size);
	inode->i_blocks = le64_to_cpu(hi->i_blocks);
	inode->i_atime.tv_sec = le64_to_cpu(hi->i_atime);
	inode->i_ctime.tv_sec = le64_to_cpu(hi->i_ctime);
	inode->i_mtime.tv_sec = le64_to_cpu(hi->i_mtime);
	inode->i_atime.tv_nsec = 0;
	inode->i_mtime.tv_nsec = 0;
	inode->i_ctime.tv_nsec = 0;
	inode->i_generation = le32_to_cpu(hi->i_generation);

	fi->i_current_depth = le32_to_cpu(hi->i_current_depth);
	fi->i_flags = le32_to_cpu(hi->i_flags);
	fi->flags = 0;
	fi->i_advise = hi->i_advise;
	fi->i_pino = le32_to_cpu(hi->i_pino);
	fi->i_blk_type = hi->i_blk_type;
	fi->i_height = hi->i_height;
	return 0;
}

void mark_size_dirty(struct inode *inode, loff_t size)
{
	struct hmfs_inode_info *hi = HMFS_I(inode);
	struct hmfs_sb_info *sbi = HMFS_I_SB(inode);

	i_size_write(inode, size);
	set_inode_flag(hi, FI_DIRTY_SIZE);
	spin_lock(&sbi->dirty_inodes_lock);
	list_del(&hi->list);
	INIT_LIST_HEAD(&hi->list);
	list_add_tail(&hi->list, &sbi->dirty_inodes_list);
	spin_unlock(&sbi->dirty_inodes_lock);
}

/*
*mark proc information has been changed
*/
void mark_proc_dirty(struct inode *inode)
{
	struct hmfs_inode_info *hi = HMFS_I(inode);
	struct hmfs_sb_info *sbi = HMFS_I_SB(inode);
	
	set_inode_flag(hi, FI_DIRTY_PROC);
	spin_lock(&sbi->dirty_inodes_lock);
	list_del(&hi->list);
	INIT_LIST_HEAD(&hi->list);
	list_add_tail(&hi->list,&sbi->dirty_inodes_list);
	spin_unlock(&sbi->dirty_inodes_lock);
}

int sync_hmfs_inode_size(struct inode *inode, bool force)
{
	struct hmfs_inode_info *inode_i = HMFS_I(inode);
	struct hmfs_sb_info *sbi = HMFS_I_SB(inode);
	struct hmfs_node *hn;
	struct hmfs_inode *hi;

	hn = alloc_new_node(sbi, inode->i_ino, inode, SUM_TYPE_INODE, force);
	if(IS_ERR(hn))
		return PTR_ERR(hn);
	hi = &hn->i;
	hi->i_size = cpu_to_le64(inode->i_size);
	hi->i_blocks = cpu_to_le64(inode->i_blocks);

	clear_inode_flag(inode_i, FI_DIRTY_SIZE);
	if (!is_inode_flag_set(inode_i, FI_DIRTY_INODE)) {
		spin_lock(&sbi->dirty_inodes_lock);
		list_del(&inode_i->list);
		spin_unlock(&sbi->dirty_inodes_lock);
		INIT_LIST_HEAD(&inode_i->list);
	}
	return 0;
}

int sync_hmfs_inode_proc(struct inode *inode, bool force)
{
	struct hmfs_sb_info *sbi = HMFS_I_SB(inode);
	struct hmfs_inode_info *inode_i = HMFS_I(inode);
	//struct hmfs_proc_info *proc= NULL;
	struct hmfs_node *hn;
	struct hmfs_inode *hi;
	int i = 0;

	hn = alloc_new_node(sbi,inode->i_ino, inode, SUM_TYPE_INODE, force);
	if(IS_ERR(hn)){
		return PTR_ERR(hn);
	} 
	hi= &hn->i;
	//proc= inode_i->i_proc_info;
	for(i=0;i<4;i++){
		hi->i_proc[i].proc_id= cpu_to_le64(inode_i->i_proc_info[i].proc_id);
		hi->i_proc[i].next_ino= cpu_to_le64(inode_i->i_proc_info[i].next_ino);
		hi->i_proc[i].next_nid= cpu_to_le64(inode_i->i_proc_info[i].next_nid);
	}

	clear_inode_flag(inode_i, FI_DIRTY_PROC);
	if(!is_inode_flag_set(inode_i,FI_DIRTY_PROC)){
		spin_lock(&sbi->dirty_inodes_lock);
		list_del(&inode_i->list);
		spin_unlock(&sbi->dirty_inodes_lock);
		INIT_LIST_HEAD(&inode_i->list);
	}
	return 0;
}

int sync_hmfs_inode(struct inode *inode, bool force)
{
	struct super_block *sb = inode->i_sb;
	struct hmfs_sb_info *sbi = HMFS_SB(sb);
	struct hmfs_inode_info *inode_i = HMFS_I(inode);
	struct hmfs_node *rn;
	struct hmfs_inode *hi;

	rn = alloc_new_node(sbi, inode->i_ino, inode, SUM_TYPE_INODE, force);
	if (IS_ERR(rn))
		return PTR_ERR(rn);
	hi = &(rn->i);

	clear_inode_flag(inode_i, FI_DIRTY_INODE);
	clear_inode_flag(inode_i, FI_DIRTY_SIZE);
	spin_lock(&sbi->dirty_inodes_lock);
	list_del(&inode_i->list);
	spin_unlock(&sbi->dirty_inodes_lock);
	INIT_LIST_HEAD(&inode_i->list);
	
	hi->i_mode = cpu_to_le16(inode->i_mode);
	hi->i_uid = cpu_to_le32(i_uid_read(inode));
	hi->i_gid = cpu_to_le32(i_gid_read(inode));
	hi->i_links = cpu_to_le32(inode->i_nlink);
	hi->i_size = cpu_to_le64(inode->i_size);
	hi->i_blocks = cpu_to_le64(inode->i_blocks);

	hi->i_atime = cpu_to_le64(inode->i_atime.tv_sec);
	hi->i_ctime = cpu_to_le64(inode->i_ctime.tv_sec);
	hi->i_mtime = cpu_to_le64(inode->i_mtime.tv_sec);
	hi->i_generation = cpu_to_le32(inode->i_generation);

	hi->i_current_depth = cpu_to_le32(inode_i->i_current_depth);
	hi->i_flags = cpu_to_le32(inode_i->i_flags);
	hi->i_pino = cpu_to_le32(inode_i->i_pino);
	hi->i_advise = inode_i->i_advise;
	hi->i_blk_type = inode_i->i_blk_type;
	hi->i_height = inode_i->i_height;

	return 0;
}

/* allocate an inode */
struct inode *hmfs_iget(struct super_block *sb, unsigned long ino)
{
	struct inode *inode;
	int ret;

	inode = iget_locked(sb, ino);
	if (!inode)
		return ERR_PTR(-ENOMEM);

	if (!(inode->i_state & I_NEW))
		return inode;

	ret = do_read_inode(inode);
	if (ret)
		goto bad_inode;
	inode->i_mapping->backing_dev_info = &hmfs_backing_dev_info;

	switch (inode->i_mode & S_IFMT) {
	case S_IFREG:
		inode->i_op = &hmfs_file_inode_operations;
		inode->i_fop = &hmfs_file_operations;
		break;
	case S_IFDIR:
		inode->i_op = &hmfs_dir_inode_operations;
		inode->i_fop = &hmfs_dir_operations;
		break;
	case S_IFLNK:
		inode->i_op = &hmfs_symlink_inode_operations;
		break;
	default:
		inode->i_op = &hmfs_special_inode_operations;
		init_special_inode(inode, inode->i_mode, inode->i_rdev);
	}
	unlock_new_inode(inode);
	//hmfs_dbg("[HMFS] : get inode #%lu\n", inode->i_ino);
	return inode;
bad_inode:
	iget_failed(inode);
	return ERR_PTR(ret);
}

//TODO
const struct address_space_operations hmfs_aops_xip = {
	.get_xip_mem = NULL,
	.direct_IO = NULL,
};
