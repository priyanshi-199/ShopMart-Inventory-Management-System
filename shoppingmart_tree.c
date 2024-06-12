#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX_ROW_WIDTH 1000
#define MAX_AISLES 10

typedef struct AVLNode
{
	char AisleName[50];
	int AisleNo;
	char ItemName[50];
	int Quantity;
	int ThresholdQuantity;
	int ExpiryYear;
	int ExpiryMonth;
	int ExpiryDate;
	int ItemId;
	int height;
	struct AVLNode *left;
	struct AVLNode *right;
} Node;

void order(Node **Aisles);
void order2(Node *root);
void rangeSearch2(Node *root, int min, int max);
void rangeSearch(Node **Aisles, int min, int max);
Node *CreateAVLNode(int ItemId, const char *AisleName, int Aisleno, const char *ItemName, int Quantity, int ThresholdQuantity, int ExpiryYear, int ExpiryMonth, int ExpiryDate)
{
	Node *nptr = (Node *)malloc(sizeof(Node));
	nptr->left = NULL;
	nptr->right = NULL;
	nptr->height = 1;
	nptr->ItemId = ItemId;
	strcpy(nptr->AisleName, AisleName);
	nptr->AisleNo = Aisleno;
	strcpy(nptr->ItemName, ItemName);
	nptr->Quantity = Quantity;
	nptr->ThresholdQuantity = ThresholdQuantity;
	nptr->ExpiryYear = ExpiryYear;
	nptr->ExpiryMonth = ExpiryMonth;
	nptr->ExpiryDate = ExpiryDate;
	return nptr;
}

int GetHeight(Node *ptr)
{
	int height;
	if (ptr == NULL)
	{
		height = 0;
	}
	else
	{
		height = ptr->height;
	}
	return height;
}

int max(int a, int b)
{
	int max;
	if (a >= b)
	{
		max = a;
	}
	else
	{
		max = b;
	}
	return max;
}
void freeTree(Node *root)
{

	if (root != NULL)
	{

		freeTree(root->left);
		freeTree(root->right);
		free(root);
	}
}
void deleteTree(Node **root)
{
	freeTree(*root);
	*root = NULL;
}

int GetBalance(Node *ptr)
{
	int balance;
	if (ptr == NULL)
	{
		balance = 0;
	}
	else
	{
		balance = GetHeight(ptr->left) - GetHeight(ptr->right);
	}
	return balance;
}

Node *RotateRight(Node *ptr1)
{
	Node *ptr2 = ptr1->left;
	Node *tptr = ptr2->right;
	ptr2->right = ptr1;
	ptr1->left = tptr;
	ptr1->height = 1 + max(GetHeight(ptr1->left), GetHeight(ptr1->right));
	ptr2->height = 1 + max(GetHeight(ptr2->left), GetHeight(ptr2->right));
	return ptr2;
}

Node *RotateLeft(Node *ptr1)
{
	Node *ptr2 = ptr1->right;
	Node *tptr = ptr2->left;
	ptr2->left = ptr1;
	ptr1->right = tptr;
	ptr1->height = 1 + max(GetHeight(ptr1->left), GetHeight(ptr1->right));
	ptr2->height = 1 + max(GetHeight(ptr2->left), GetHeight(ptr2->right));
	return ptr2;
}

Node *Insert(Node *ptr, int ItemId, const char *AisleName, int AisleNo, const char *ItemName, int Quantity, int ThresholdQuantity, int ExpiryYear, int ExpiryMonth, int ExpiryDate)
{
	if (ptr == NULL)
	{
		ptr = CreateAVLNode(ItemId, AisleName, AisleNo, ItemName, Quantity, ThresholdQuantity, ExpiryYear, ExpiryMonth, ExpiryDate);
	}
	else
	{
		if (ItemId < ptr->ItemId)
		{
			ptr->left = Insert(ptr->left, ItemId, AisleName, AisleNo, ItemName, Quantity, ThresholdQuantity, ExpiryYear, ExpiryMonth, ExpiryDate);
		}
		else if (ItemId > ptr->ItemId)
		{
			ptr->right = Insert(ptr->right, ItemId, AisleName, AisleNo, ItemName, Quantity, ThresholdQuantity, ExpiryYear, ExpiryMonth, ExpiryDate);
		}
		if (ptr->ItemId != ItemId)
		{
			ptr->height = 1 + max(GetHeight(ptr->left), GetHeight(ptr->right));
			int balance = GetBalance(ptr);
			if (balance > 1 && ItemId < ptr->left->ItemId) // LL
			{
				ptr = RotateRight(ptr);
			}
			else if (balance < -1 && ItemId > ptr->right->ItemId) // RR
			{
				ptr = RotateLeft(ptr);
			}
			else if (balance > 1 && ItemId > ptr->left->ItemId) // LR
			{
				ptr->left = RotateLeft(ptr->left);
				ptr = RotateRight(ptr);
			}
			else if (balance < -1 && ItemId < ptr->right->ItemId) // RL
			{
				ptr->right = RotateRight(ptr->right);
				ptr = RotateLeft(ptr);
			}
		}
	}
	return ptr;
}
void visit(Node *nptr)
{
	if (nptr != NULL)
	{
		printf(" ItemId:%d, ItemName:%s, Quantity:%d, ThresholdQuantity:%d, ExpiryYear:%d, ExpiryMonth:%d, ExpiryDate:%d\n",
		       nptr->ItemId, nptr->ItemName, nptr->Quantity, nptr->ThresholdQuantity,
		       nptr->ExpiryYear, nptr->ExpiryMonth, nptr->ExpiryDate);
	}
}
void PreOrder(Node *root)
{
	if (root != NULL)
	{
		visit(root);
		PreOrder(root->left);
		PreOrder(root->right);
	}
	return;
}
void PostOrder(Node *root)
{
	if (root != NULL)
	{

		PostOrder(root->left);
		PostOrder(root->right);
		visit(root);
	}
	return;
}
void InOrder(Node *root)
{
	if (root != NULL)
	{

		InOrder(root->left);
		visit(root);
		InOrder(root->right);
	}
	return;
}

Node **readDataFromFile(const char *filename, int *numAisles)
{
	FILE *file = fopen(filename, "r");
	if (!file)
	{
		printf("Error opening file.\n");
		return NULL;
	}

	char row[MAX_ROW_WIDTH];
	int maxAisleNo = 0;

	maxAisleNo = MAX_AISLES;
	*numAisles = maxAisleNo;

	Node **aisles = (Node **)malloc((maxAisleNo + 1) * sizeof(Node *));
	if (aisles == NULL)
	{
		printf("Memory allocation failed.\n");
		fclose(file);
		return NULL;
	}

	for (int i = 0; i <= maxAisleNo; i++)
	{
		aisles[i] = NULL;
	}

	while (fgets(row, sizeof(row), file))
	{
		int AisleNo, ItemId, Quantity, ThresholdQuantity, ExpiryYear, ExpiryMonth, ExpiryDate;
		char ItemName[50];
		char AisleName[50];
		sscanf(row, "%d,%[^,],%d,%[^,],%d,%d,%d,%d,%d", &AisleNo, AisleName, &ItemId, ItemName, &Quantity, &ThresholdQuantity, &ExpiryYear, &ExpiryMonth, &ExpiryDate);
		if (AisleNo <= MAX_AISLES)
			aisles[AisleNo - 1] = Insert(aisles[AisleNo - 1], ItemId, AisleName, AisleNo, ItemName, Quantity, ThresholdQuantity, ExpiryYear, ExpiryMonth, ExpiryDate);
	}

	fclose(file);

	return aisles;
}
Node *DeleteNode(Node **npptr)
{
	Node *nptr, *prev, *ptr;
	nptr = *npptr;
	if (nptr == NULL)
	{
		printf("Error:<Empty Node");
	}
	else if (nptr->right == NULL)
	{
		*npptr = nptr->left;
		free(nptr);
	}
	else if (nptr->left == NULL)
	{
		*npptr = nptr->right;
		free(nptr);
	}
	else
	{
		prev = ptr = nptr->left;
		while (ptr->right != NULL)
		{
			prev = ptr;
			ptr = ptr->right;
		}
		if (prev == ptr)
		{
			ptr->right = nptr->right;
			*npptr = ptr;
			free(nptr);
		}
		else
		{
			prev->right = ptr->left;
			ptr->left = nptr->left;
			ptr->right = nptr->right;
			*npptr = ptr;
			free(nptr);
		}
	}
	return *npptr;
}

Node *deleteN(Node *ptr, int ItemId)
{
	if (ptr == NULL)
	{
		return ptr;
	}
	else
	{
		if (ItemId < ptr->ItemId)
		{
			ptr->left = deleteN(ptr->left, ItemId);
		}
		else if (ItemId > ptr->ItemId)
		{
			ptr->right = deleteN(ptr->right, ItemId);
		}
		else
		{
			ptr = DeleteNode(&ptr);
		}
		if (ptr != NULL && ptr->ItemId != ItemId)
		{
			ptr->height = 1 + max(GetHeight(ptr->left), GetHeight(ptr->right));
			int balance = GetBalance(ptr);
			if (balance > 1 && GetBalance(ptr->left) >= 0) // LL
			{
				ptr = RotateRight(ptr);
			}
			else if (balance > 1 && GetBalance(ptr->left) < 0) // LR
			{
				ptr->left = RotateLeft(ptr->left);
				ptr = RotateRight(ptr);
			}
			else if (balance < -1 && GetBalance(ptr->right) > 0) // RL
			{
				ptr->right = RotateRight(ptr->right);
				ptr = RotateLeft(ptr);
			}
			else if (balance < -1 && GetBalance(ptr->right) <= 0) // RR
			{
				ptr = RotateLeft(ptr);
			}
		}
	}
	return ptr;
}

Node **add(const char *filename, Node **Aisles, int numAisles)
{
	FILE *file = fopen(filename, "r");
	if (!file)
	{
		printf("Error opening file.\n");
		return Aisles;
	}

	char row[MAX_ROW_WIDTH];

	while (fgets(row, sizeof(row), file))
	{

		int AisleNo, ItemId, Quantity, ThresholdQuantity, ExpiryYear, ExpiryMonth, ExpiryDate;
		char ItemName[50];
		char AisleName[50];
		if (sscanf(row, "%d,%[^,],%d,%[^,],%d,%d,%d,%d,%d", &AisleNo, AisleName, &ItemId, ItemName, &Quantity, &ThresholdQuantity, &ExpiryYear, &ExpiryMonth, &ExpiryDate) == 9)
		{
			if (AisleNo <= numAisles)
			{

				Node *nptr = CreateAVLNode(ItemId, AisleName, AisleNo, ItemName, Quantity, ThresholdQuantity, ExpiryYear, ExpiryMonth, ExpiryDate);
				if (nptr == NULL)
				{
					printf("Error creating node.\n");
					continue;
				}
				Aisles[AisleNo - 1] = Insert(Aisles[AisleNo - 1], ItemId, AisleName, AisleNo, ItemName, Quantity, ThresholdQuantity, ExpiryYear, ExpiryMonth, ExpiryDate);
			}
		}
	}

	fclose(file);

	return Aisles;
}
Node **readAndAddNewAislesFromFile(const char *filename, Node **aisles, int maxAisles)
{
	FILE *file = fopen(filename, "r");
	if (!file)
	{
		printf("Error opening file.\n");
		return aisles;
	}
	int flag = 0, i, n;
	for ((i = 1); i < MAX_AISLES && (flag == 0); i++)
	{
		if (aisles[i - 1] == NULL)
		{
			n = i;
			flag = 1;
		}
	}
	char row[MAX_ROW_WIDTH];
	fgets(row, sizeof(row), file);

	while (fgets(row, sizeof(row), file))
	{
		int AisleNo, ItemId, Quantity, ThresholdQuantity, ExpiryYear, ExpiryMonth, ExpiryDate;
		char ItemName[50];
		char AisleName[50];
		if (sscanf(row, "%d,%[^,],%d,%[^,],%d,%d,%d,%d,%d", &AisleNo, AisleName, &ItemId, ItemName, &Quantity, &ThresholdQuantity, &ExpiryYear, &ExpiryMonth, &ExpiryDate) == 9)
		{
			aisles[n - 1] = Insert(aisles[n - 1], ItemId, AisleName, AisleNo, ItemName, Quantity, ThresholdQuantity, ExpiryYear, ExpiryMonth, ExpiryDate);
		}
	}

	fclose(file);
	return aisles;
}
void order(Node **Aisles)
{
	printf("\nItems to order:");
	for (int i = 0; i < MAX_AISLES; i++)
	{
		Node *ptr = Aisles[i];
		if (ptr != NULL)
		{
			order2(Aisles[i]);
		}
	}
}
void order2(Node *ptr)
{
	if (ptr != NULL)
	{
		if (ptr->ThresholdQuantity > ptr->Quantity)
		{
			printf("\n%s", ptr->ItemName);
		}
		order2(ptr->left);
		order2(ptr->right);
	}
}

void rangeSearch(Node **Aisles, int min, int max)
{
	Node *root;
	int i;
	for (i = 0; i < MAX_AISLES; i++)
	{
		root = Aisles[i];
		if (root != NULL)
		{
			rangeSearch2(root, min, max);
		}
	}
}

void rangeSearch2(Node *root, int min, int max)
{
	if (root == NULL)
		return;

	if (min < root->ItemId)
		rangeSearch2(root->left, min, max);

	if (root->ItemId >= min && root->ItemId <= max)
	{

		printf("ItemId:%d\tName:%s\n", root->ItemId, root->ItemName);
	}

	if (max > root->ItemId)
		rangeSearch2(root->right, min, max);
}

void Search(Node **Aisles, int ItemId, int Quantity, int d, int m, int y)
{
	Node *root;
	int flag = 0;

	for (int i = 0; i < MAX_AISLES && flag == 0; i++)
	{
		root = Aisles[i];

		while (root != NULL && flag == 0)
		{
			if (root->ItemId > ItemId)
			{

				root = root->left;
			}
			else if (root->ItemId < ItemId)
			{

				root = root->right;
			}
			else
			{
				flag = 1;
				if (root->Quantity >= Quantity)
				{
					if (root->ExpiryYear > y)
					{
						printf("Item %s is available\n", root->ItemName);
					}
					else if (root->ExpiryYear == y)
					{
						if (root->ExpiryMonth > m)
						{
							printf("Item %s is available\n", root->ItemName);
						}
						else if (root->ExpiryMonth == m && root->ExpiryDate > d)
						{
							printf("Item %s is available\n", root->ItemName);
						}
						else
						{
							printf("Item %s not available\n", root->ItemName);
						}
					}
					else
					{
						printf("Item %s not available\n", root->ItemName);
					}
				}
				else
				{
					printf("Item %s not available\n", root->ItemName);
				}
			}
		}
	}
	if (root == NULL)
	{
		printf("Item not found");
	}
}
void ReadyToEat_exp(Node *ptr, int maxAisleNo)
{

	int days;
	int flag = 0;
	int d, m, y;
	time_t t = time(NULL);
	struct tm *current_time = localtime(&t);
	d = current_time->tm_mday;
	m = current_time->tm_mon + 1;
	y = current_time->tm_year + 1900;

	if (ptr == NULL)
		return;

	days = 0;
	if (ptr->ExpiryYear == y)
	{
		if (ptr->ExpiryMonth == m)
		{
			days = ptr->ExpiryDate - d;
		}
		else
		{
			if ((m == 1 || m == 3 || m == 5 || m == 7 || m == 8 || m == 10 || m == 12) && (ptr->ExpiryMonth - m == 1))
			{
				days = 31 - d + ptr->ExpiryDate;
			}
			else if ((m == 4 || m == 6 || m == 9 || m == 11) && (ptr->ExpiryMonth - m == 1))
			{
				days = 30 - d + ptr->ExpiryDate;
			}
			else if ((m == 2) && (ptr->ExpiryMonth == 3))
			{
				if (y % 4 == 0)
				{
					days = 29 - d + ptr->ExpiryDate;
				}
				else
				{
					days = 28 - d + ptr->ExpiryDate;
				}
			}
		}
	}
	else if (ptr->ExpiryYear - y == 1)
	{
		if ((m == 12))
		{
			days = 31 - d + ptr->ExpiryDate;
		}
	}

	if (((days > 0) && (days <= 7)) || ((ptr->ExpiryYear == y) && (ptr->ExpiryMonth == m) && (ptr->ExpiryDate == d)))
	{
		printf("\n%s is about to expire within %d days", ptr->ItemName, days);
		flag = 1;
	}

	ReadyToEat_exp(ptr->left, maxAisleNo);
	ReadyToEat_exp(ptr->right, maxAisleNo);
}

Node *mergeAVL(Node *root1, Node *root2)
{
	if (root2 != NULL)
	{
		root1 = Insert(root1, root2->ItemId, root2->AisleName, root2->AisleNo, root2->ItemName, root2->Quantity, root2->ThresholdQuantity, root2->ExpiryYear, root2->ExpiryMonth, root2->ExpiryDate);
		root1 = mergeAVL(root1, root2->left);
		root1 = mergeAVL(root1, root2->right);
	}
	return root1;
}
typedef struct ItemNode
{
	char name[100];
	int id;
	int quantity;
	float price;
	int AisleNo;
	int height;

	struct ItemNode *left;
	struct ItemNode *right;
} ItemNode;

typedef struct BillNode
{
	int billNumber;
	int height;
	struct ItemNode *itemHead;
	struct BillNode *left;
	struct BillNode *right;
} BillNode;

void itemsearch(int targetItemId, ItemNode *itemptr, BillNode *currentBill, int targetAisleno, int *flag1);
void frequencytree(ItemNode *itemptr2, int TargetItemId, int targetAisleno);

int GetHeightB(BillNode *ptr)
{
	int height;
	if (ptr == NULL)
	{
		height = 0;
	}
	else
	{
		height = ptr->height;
	}
	return height;
}
int GetHeightI(ItemNode *ptr)
{
	int height;
	if (ptr == NULL)
	{
		height = 0;
	}
	else
	{
		height = ptr->height;
	}
	return height;
}

int GetBalanceB(BillNode *ptr)
{
	int balance;
	if (ptr == NULL)
	{
		balance = 0;
	}
	else
	{
		balance = GetHeightB(ptr->left) - GetHeightB(ptr->right);
	}
	return balance;
}
int GetBalanceI(ItemNode *ptr)
{
	int balance;
	if (ptr == NULL)
	{
		balance = 0;
	}
	else
	{
		balance = GetHeightI(ptr->left) - GetHeightI(ptr->right);
	}
	return balance;
}

BillNode *RotateRightB(BillNode *ptr1)
{
	BillNode *ptr2 = ptr1->left;
	BillNode *tptr = ptr2->right;
	ptr2->right = ptr1;
	ptr1->left = tptr;
	ptr1->height = 1 + max(GetHeightB(ptr1->left), GetHeightB(ptr1->right));
	ptr2->height = 1 + max(GetHeightB(ptr2->left), GetHeightB(ptr2->right));
	return ptr2;
}
ItemNode *RotateRightI(ItemNode *ptr1)
{
	ItemNode *ptr2 = ptr1->left;
	ItemNode *tptr = ptr2->right;
	ptr2->right = ptr1;
	ptr1->left = tptr;
	ptr1->height = 1 + max(GetHeightI(ptr1->left), GetHeightI(ptr1->right));
	ptr2->height = 1 + max(GetHeightI(ptr2->left), GetHeightI(ptr2->right));
	return ptr2;
}

BillNode *RotateLeftB(BillNode *ptr1)
{
	BillNode *ptr2 = ptr1->right;
	BillNode *tptr = ptr2->left;
	ptr2->left = ptr1;
	ptr1->right = tptr;
	ptr1->height = 1 + max(GetHeightB(ptr1->left), GetHeightB(ptr1->right));
	ptr2->height = 1 + max(GetHeightB(ptr2->left), GetHeightB(ptr2->right));
	return ptr2;
}
ItemNode *RotateLeftI(ItemNode *ptr1)
{
	ItemNode *ptr2 = ptr1->right;
	ItemNode *tptr = ptr2->left;
	ptr2->left = ptr1;
	ptr1->right = tptr;
	ptr1->height = 1 + max(GetHeightI(ptr1->left), GetHeightI(ptr1->right));
	ptr2->height = 1 + max(GetHeightI(ptr2->left), GetHeightI(ptr2->right));
	return ptr2;
}

ItemNode *createItemNode(int id, char name[], int quantity, float price, int AisleNo)
{
	ItemNode *newNode = (ItemNode *)malloc(sizeof(ItemNode));
	if (newNode == NULL)
	{
		printf("Memory allocation failed\n");
		exit(1);
	}
	newNode->id = id;
	newNode->height = 1;
	strcpy(newNode->name, name);
	newNode->quantity = quantity;
	newNode->price = price;
	newNode->AisleNo = AisleNo;
	newNode->left = NULL;
	newNode->right = NULL;
	return newNode;
}

BillNode *createBillNode(int billNumber)
{
	BillNode *newNode = (BillNode *)malloc(sizeof(BillNode));
	if (newNode == NULL)
	{
		printf("Memory allocation failed\n");
		exit(1);
	}
	newNode->billNumber = billNumber;
	newNode->height = 1;
	newNode->itemHead = NULL;
	newNode->left = NULL;
	newNode->right = NULL;
	return newNode;
}
typedef struct FrequencyNode
{
	int id;
	char name[100];
	int frequency;
	struct FrequencyNode *next;
} FrequencyNode;
FrequencyNode *createFrequencyNode(int id, char name[])
{
	FrequencyNode *newNode = (FrequencyNode *)malloc(sizeof(FrequencyNode));
	if (newNode == NULL)
	{
		printf("Memory allocation failed\n");
		exit(1);
	}
	newNode->id = id;
	strcpy(newNode->name, name);
	newNode->frequency = 1;
	newNode->next = NULL;
	return newNode;
}
FrequencyNode *Freqptr = NULL;

FrequencyNode *addFrequency(FrequencyNode *head, int id, char name[])
{
	FrequencyNode *current = head;
	while (current != NULL)
	{
		if (current->id == id)
		{
			current->frequency++;
			return head;
		}
		current = current->next;
	}
	FrequencyNode *newNode = createFrequencyNode(id, name);
	newNode->next = head;
	return newNode;
}

BillNode *addBill(BillNode *head, int billNumber)
{
	if (head == NULL)
	{
		head = createBillNode(billNumber);
	}
	else
	{
		if (billNumber < head->billNumber)
		{
			head->left = addBill(head->left, billNumber);
		}
		else if (billNumber > head->billNumber)
		{
			head->right = addBill(head->right, billNumber);
		}
		if (head->billNumber != billNumber)
		{
			head->height = 1 + max(GetHeightB(head->left), GetHeightB(head->right));
			int balance = GetBalanceB(head);
			if (balance > 1 && billNumber < head->left->billNumber) // LL
			{
				head = RotateRightB(head);
			}
			else if (balance < -1 && billNumber > head->right->billNumber) // RR
			{
				head = RotateLeftB(head);
			}
			else if (balance > 1 && billNumber > head->left->billNumber) // LR
			{
				head->left = RotateLeftB(head->left);
				head = RotateRightB(head);
			}
			else if (balance < -1 && billNumber < head->right->billNumber) // RL
			{
				head->right = RotateRightB(head->right);
				head = RotateLeftB(head);
			}
		}
	}
	return head;
}

ItemNode *addItem(ItemNode *head, int id, char name[], int quantity, float price, int AisleNo)
{
	if (head == NULL)
	{
		head = createItemNode(id, name, quantity, price, AisleNo);
	}
	else
	{
		if (id < head->id)
		{
			head->left = addItem(head->left, id, name, quantity, price, AisleNo);
		}
		else if (id > head->id)
		{
			head->right = addItem(head->right, id, name, quantity, price, AisleNo);
		}
		else
		{

			head->quantity += quantity;
			head->price = price;
		}

		head->height = 1 + max(GetHeightI(head->left), GetHeightI(head->right));
		int balance = GetBalanceI(head);
		if (balance > 1 && id < head->left->id) // LL
		{
			head = RotateRightI(head);
		}
		else if (balance < -1 && id > head->right->id) // RR
		{
			head = RotateLeftI(head);
		}
		else if (balance > 1 && id > head->left->id) // LR
		{
			head->left = RotateLeftI(head->left);
			head = RotateRightI(head);
		}
		else if (balance < -1 && id < head->right->id) // RL
		{
			head->right = RotateRightI(head->right);
			head = RotateLeftI(head);
		}
	}
	return head;
}

void visititem(ItemNode *nptr)
{
	if (nptr != NULL)
	{
		printf(" ItemId:%d, ItemName:%s, MRP:%d \n", nptr->id, nptr->name, nptr->price);
	}
}

void InOrderItem(ItemNode *head)
{
	if (head != NULL)
	{
		InOrderItem(head->left);
		visititem(head);
		InOrderItem(head->right);
	}
}
void PreOrderItem(ItemNode *head)
{
	if (head != NULL)
	{
		visititem(head);
		PreOrderItem(head->left);

		PreOrderItem(head->right);
	}
}
void InOrderBill(BillNode *root)
{
	if (root != NULL)
	{
		InOrderItem(root->itemHead);
		printf("\n");
		InOrderBill(root->left);

		InOrderBill(root->right);
	}
	return;
}
void PreOrderBill(BillNode *root)
{
	if (root != NULL)
	{

		PreOrderBill(root->left);
		PreOrderItem(root->itemHead);
		printf("\n");
		PreOrderBill(root->right);
	}
	return;
}

BillNode *findBill(BillNode *root, int billNumber)
{

	if (root == NULL || root->billNumber == billNumber)
	{
		return root;
	}
	else if (billNumber < root->billNumber)
	{
		return findBill(root->left, billNumber);
	}
	else
	{
		return findBill(root->right, billNumber);
	}
}

BillNode *readBillsFromFileB(const char *filename)
{

	BillNode *billList = NULL;
	FILE *file = fopen(filename, "r");
	if (file == NULL)
	{
		printf("Error opening file\n");
		exit(1);
	}

	char line[1024];
	while (fgets(line, sizeof(line), file))
	{

		int billNumber, id, quantity, aisleNo;
		char name[100];
		float price;
		if (sscanf(line, "%d,%d,%[^,],%d,%f,%d", &billNumber, &id, name, &quantity, &price, &aisleNo) != 6)
		{

			continue;
		}

		BillNode *currentBill = findBill(billList, billNumber);
		if (currentBill == NULL)
		{

			billList = addBill(billList, billNumber);
			currentBill = findBill(billList, billNumber);
		}

		currentBill->itemHead = addItem(currentBill->itemHead, id, name, quantity, price, aisleNo);
	}

	fclose(file);
	return billList;
}

FrequencyNode *freqroot = NULL;

int flag1;

void findItemBoughtTogether(BillNode *currentBill, int targetItemId, int targetAisleno)
{
	if (currentBill != NULL)
	{
		flag1 = 0;

		ItemNode *itemptr = currentBill->itemHead;

		itemsearch(targetItemId, itemptr, currentBill, targetAisleno, &flag1);
		findItemBoughtTogether(currentBill->left, targetItemId, targetAisleno);
		findItemBoughtTogether(currentBill->right, targetItemId, targetAisleno);
	}
}

void itemsearch(int targetItemId, ItemNode *itemptr, BillNode *currentBill, int targetAisleno, int *flag1)
{
	int targetAisleNo;
	if (itemptr != NULL && *flag1 == 0)
	{

		if (itemptr->id == targetItemId)
		{

			frequencytree(currentBill->itemHead, targetItemId, targetAisleno);
			*flag1 = 1;
			return;
		}
		else if (targetItemId > itemptr->id)

		{

			itemsearch(targetItemId, itemptr->right, currentBill, targetAisleno, flag1);
		}
		else
		{

			itemsearch(targetItemId, itemptr->left, currentBill, targetAisleno, flag1);
		}
	}
}

void frequencytree(ItemNode *itemptr2, int TargetItemId, int targetAisleno)
{
	freqroot = NULL;

	if (itemptr2 != NULL)
	{

		if (itemptr2->id != TargetItemId)
		{

			Freqptr = addFrequency(Freqptr, itemptr2->id, itemptr2->name);
		}
		frequencytree(itemptr2->left, TargetItemId, targetAisleno);
		frequencytree(itemptr2->right, TargetItemId, targetAisleno);
	}
}
void recommend(FrequencyNode *ptr)
{
	int trend = 0;
	while (ptr != NULL)
	{
		if (ptr->frequency >= 2)
		{
			printf("%d:%s\n", ptr->id, ptr->name);
			trend = 1;
		}
		ptr = ptr->next;
	}
	if (trend == 0)
	{
		printf("No proper trend is observed\n");
	}
}
void writeDataToCSV(FILE *file, int aisleno, Node *root);
void copyDataToCSV(const char *filename, Node **aisles, int numAisles)
{
	FILE *file = fopen(filename, "w");
	if (!file)
	{
		printf("Error opening file for writing.\n");
		return;
	}

	fprintf(file, "AisleNo,AisleName,ItemId,ItemName,Quantity,ThresholdQuantity,ExpiryYear,ExpiryMonth,ExpiryDate\n");

	for (int i = 0; i < numAisles; ++i)
	{
		Node *current = aisles[i];

		writeDataToCSV(file, i + 1, current);
	}

	fclose(file);
}

void writeDataToCSV(FILE *file, int aisleno, Node *root)
{
	if (root == NULL)
		return;

	writeDataToCSV(file, aisleno, root->left);

	fprintf(file, "%d,%s,%d,%s,%d,%d,%d,%d,%d\n",
		aisleno, root->AisleName, root->ItemId, root->ItemName, root->Quantity, root->ThresholdQuantity,
		root->ExpiryYear, root->ExpiryMonth, root->ExpiryDate);

	writeDataToCSV(file, aisleno, root->right);
}
void billing2(ItemNode *Itemroot, Node **Aisles);

void billing3(ItemNode *Itemptr, Node *Aisleroot);

void billing4(ItemNode *Itemptr, Node *Aisleitemptr);

void billing(BillNode *billList, Node **Aisles)
{
	BillNode *ptr = billList;
	if (ptr != NULL)
	{
		billing(ptr->left, Aisles);
		billing2(ptr->itemHead, Aisles);
		billing(ptr->right, Aisles);
	}
}
void billing2(ItemNode *Itemroot, Node **Aisles)
{
	ItemNode *ptr = Itemroot;
	if (ptr != NULL)
	{

		billing2(ptr->left, Aisles);

		billing3(ptr, Aisles[ptr->AisleNo - 1]);
		billing2(ptr->right, Aisles);
	}
}
void billing3(ItemNode *Itemptr, Node *Aisleroot)
{
	Node *ptr = Aisleroot;
	if (ptr != NULL)
	{
		billing3(Itemptr, ptr->left);
		if (Itemptr->id == ptr->ItemId)
		{
			billing4(Itemptr, ptr);
		}
		billing3(Itemptr, ptr->right);
	}
}
void billing4(ItemNode *Itemptr, Node *Aisleitemptr)
{
	Aisleitemptr->Quantity = Aisleitemptr->Quantity - Itemptr->quantity;
}

int main()
{
	printf("Welcome to shopmart store!!!");
	printf("\n----------------------------------------------------------------------------------------------\n");

	int numAisles;
	int *result = NULL;
	int index = 0;

	Node **aisles = readDataFromFile("shopmartdata.csv", &numAisles);
	if (aisles == NULL)
	{
		printf("Failed to read inventory data from file.\n");
		return 1;
	}

	aisles = add("add_item.csv", aisles, numAisles);

	printf("New items are added to file.");
	printf("\n----------------------------------------------------------------------------------------------\n");
	int delaisle;
	printf("Enter aisle  no to be deleted:");
	scanf("%d", &delaisle);
	deleteTree(&aisles[delaisle - 1]);
	printf("Aisle is deleted from file.");
	printf("\n----------------------------------------------------------------------------------------------\n");

	aisles = readAndAddNewAislesFromFile("Add_aisle.csv", aisles, numAisles);
	printf("New Aisle data is added");
	printf("\n----------------------------------------------------------------------------------------------\n");
	printf("\nFollowing ready to eat items are going to expire within 7 days:");
	ReadyToEat_exp(aisles[3], numAisles);
	printf("\n----------------------------------------------------------------------------------------------\n");
	int delid, delais;
	printf("\n\nEnter details  of item to be deleted-\nEnter ItemId:");
	scanf("%d", &delid);
	printf("Enter AisleNo:");
	scanf("%d", &delais);
	aisles[delais - 1] = deleteN(aisles[delais - 1], delid);
	printf("Item is deleted from file");
	printf("\n----------------------------------------------------------------------------------------------\n");
	int searchid, searchqty, searchd, searchm, searchy;
	printf("\nEnter details of item id to be searched-\nEnter ItemId:");
	scanf("%d", &searchid);
	printf("\nEnter required Quantity:");
	scanf("%d", &searchqty);
	printf("\nEnter required expiry in DD MM YY format:");
	scanf("%d%d%d", &searchd, &searchm, &searchy);
	Search(aisles, searchid, searchqty, searchd, searchm, searchy);
	printf("\n----------------------------------------------------------------------------------------------\n");
	order(aisles);
	printf("\n----------------------------------------------------------------------------------------------\n");
	printf("\nEnter min and max for range search:");
	int min;
	int max;
	scanf("%d%d", &min, &max);
	printf("\nElements within the range [%d, %d]:\n ", min, max);
	rangeSearch(aisles, min, max);
	printf("\n----------------------------------------------------------------------------------------------\n");
	BillNode *billList = readBillsFromFileB("bill.csv");
	if (billList == NULL)
	{
		printf("Failed to read bill list\n");
		return 1;
	}
	printf("\nrecommended item:\n");

	findItemBoughtTogether(billList, 102, 1);

	recommend(Freqptr);
	printf("\n----------------------------------------------------------------------------------------------\n");
	printf("Stock is updated");
	billing(billList, aisles);
	printf("\n----------------------------------------------------------------------------------------------\n");

	aisles[3] = mergeAVL(aisles[3], aisles[4]);
	deleteTree(&aisles[4]);
	aisles = readAndAddNewAislesFromFile("new_aisle.csv", aisles, numAisles);
	printf("Two aisles are merged and and new aisle is added.");
	printf("\n----------------------------------------------------------------------------------------------\n");
	printf("Thank you!!");
	copyDataToCSV("file.csv", aisles, numAisles);

	for (int i = 0; i < numAisles; i++)
	{
		freeTree(aisles[i]);
		aisles[i] = NULL;
	}
	free(aisles);
	return 0;
}