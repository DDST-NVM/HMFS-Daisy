struct node{
	int num;
	struct node *next;
};

void initList(struct node *start){
	start->next=NULL;
}

int insertLastList(struct node *pNode,int insertElem)
{
    struct node *pInsert;
    struct node *pHead;
   // struct node *pTmp; //定义一个临时链表用来存放第一个节点
 
    pHead = pNode;
   // pTmp = pHead;
    pInsert = (struct node *)malloc(sizeof(struct node)); //申请一个新节点
  //  memset(pInsert,0,sizeof(struct node));
    pInsert->next = NULL;
    pInsert->num = insertElem;
 
    while(pHead->next != NULL)
    {
        pHead = pHead->next;
    }
    pHead->next = pInsert;   //将链表末尾节点的下一结点指向新添加的节点
 //   *pNode = pTmp;
  //  printf("insertLastList函数执行，向表尾插入元素成功\n");
 
    return 1;
}
int Del_Node(struct node *pNode,int back)
{

    int i = 0;
    int data;
    struct node *pDel = pNode;
    struct node *pSwap;
    if ((back < 1) && (NULL == pDel->next))
    {
        printf("删除失败！\n");
        return 0;
    }
    while(i < back-1)
    {
        pDel = pDel->next;
        ++i;
    }
    pSwap = pDel->next;
    data = pSwap->num;
    pDel->next = pDel->next->next;
    free(pSwap);
    return data;
}
