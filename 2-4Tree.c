/********************************************************/
/*			2-4Tree made by Kim Yoonhyeon				*/
/*			For Algorithm Assignment					*/
/********************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define maxLength 1000		// Maximum number of nodes which can be printed in one row.
#define TRUE 1
#define FALSE 0

typedef struct node {
	struct node *pNode;		// Parent node

	struct node *cNode[4];	// Children node

	int val[3];				// Values (-1: empty)

}node;

/* Function Difine */
node* makeNode();
void insert(node *, int);
void delete(node *, int);
int search(node *, int);
void propagate(node *);
node* split(node *, node *);
int putSuccessor(node *, int);
void reconst(node *);
void transferR(node *, node *);
void transferL(node *, node *);
void fusion(node *, node *);
void equalizeNode(node *);
int findNum(node *);
void printTree(node *);
void printNode(node *);
void fprintTree(node *);
void fpirntNode(node *);


node *root;
node *nodeA;
int height;
int mode;
FILE *input, *output;

int main(int argc, char* argv[]) {
	char cin;
	int iin;
	char *fileString;

	/* Print Welcome Message */
	printf("****************************************************\n");
	printf("**************  WELCOME TO 2-4 TREE!  **************\n");
	printf("*************  code by Kim Yoonhyeon  **************\n");
	printf("****************************************************\n\n");

	/* Build 2-4 Tree */
	root = makeNode();
	height = 1;
	
	/* Mode Select */
	printf("#  Select Mode\n1. Interactive mode\n2. Batch mode\n");
	printf("Your Input: ");

	scanf_s("%d", &mode);

	/* Batch mode */
	if (mode == 2) {
		printf("#  Batch mode\n");

		/* Parameter Check */
		if (argc != 2) {
			printf("No parameter\n");
			exit(1);
		}
		
		/* Open File */
		fileString = argv[1];
		if (fopen_s(&input, fileString, "r") != 0) {
			printf("No %s", fileString);
			exit(1);
		}
		if (fopen_s(&output, "output.txt", "w") != 0) {
			printf("Error");
			exit(1);
		}

	}

	/* Batch mode Main Loop */
	while (mode == 2) {
		/* Read file and check file is ended or not */
		if(EOF == fscanf_s(input, "%c%d ", &cin, 1, &iin)){
			/* Close files */
			fclose(input);
			fclose(output);

			/* Print Massage and Exit Program */
			printf("Done. Check File \"output.txt\"\n");
			return 0;
		}

		/* Insert and Write Tree */
		if (cin == 'i') {
			insert(root, iin);
			fprintf(output, "i%d\n", iin);
			fprintTree(root);
		}

		/* Search and Write Tree*/
		else if (cin == 's') {
			fprintf(output, "s%d\n", iin);
			if (search(root, iin) != -1)
				fprintf(output, "exist\n");
			else
				fprintf(output, "not exits\n");
		}

		/* Delete and Write Tree */
		else if (cin == 'd') {
			delete(root, iin);
			fprintf(output, "d%d\n", iin);
			fprintTree(root);
		}
		fprintf(output, "\n");
	}


	/* Interactive mode */
	if (mode == 1) {
		printf("\n#  Interactive mode\n");
		printf("i: insertion\nd: deletion\ns: searching\ne0: exit\n");
	}

	/* Interactive mode Main Loop */
	while (mode == 1) {
		/* Read Command */
		scanf_s("%c%d", &cin, 1, &iin);

		/* Insert and Print Tree */
		if (cin == 'i') {
			insert(root, iin);
			printf("i%d\n", iin);
			printTree(root);
		}

		/* Search and Print Tree */
		else if (cin == 's') {
			printf("s%d\n", iin);
			if (search(root, iin) != -1)
				printf("exist\n");
			else
				printf("not exits\n");
		}

		/* Delete and Print Tree */
		else if (cin == 'd') {
			delete(root, iin);
			printf("d%d\n", iin);
			printTree(root);
		}

		/* Exit */
		else if (cin == 'e') {
			if (iin == 0)
				break;
		}
		else 
			continue;

		printf("\n");
	}
	return 0;
}


/************************************************************/
/* <<makeNode>>	make an empty node							*/
/*	Input: NONE												*/
/*	Output: (node *) empty node								*/
/************************************************************/
node* makeNode() {
	node *mNode = NULL;
	mNode = (node *)malloc(sizeof(node));	// Memory Allocate for a node

	/* Initial value is -1 (empty) */
	mNode->val[0] = -1;
	mNode->val[1] = -1;
	mNode->val[2] = -1;

	/* Initialize child nodes */
	mNode->pNode = NULL;
	mNode->cNode[0] = NULL;
	mNode->cNode[1] = NULL;
	mNode->cNode[2] = NULL;
	mNode->cNode[3] = NULL;

	return mNode;
}
/************************************************************/
/* <<insert>>	Insert a value								*/
/*	Input: (node *) insert location, (int) inserted value	*/
/*	Output: NONE											*/
/************************************************************/
void insert(node *mNode, int val) {
	node *temp;

	/* if the node is a Leaf */
	if (mNode->cNode[0] == NULL) {
		/* Insert a value (empty node) */
		if (mNode->val[0] == -1) {
			mNode->val[0] = val;
		}
		/* Insert a value in a right location (2-node) */
		else if (mNode->val[1] == -1) {
			if (mNode->val[0] > val) {
				mNode->val[1] = mNode->val[0];
				mNode->val[0] = val;
			}
			else {
				mNode->val[1] = val;
			}
		}
		/* Insert a value in a right location (3-node) */
		else if (mNode->val[2] == -1) {
			if (mNode->val[0] > val) {
				mNode->val[2] = mNode->val[1];
				mNode->val[1] = mNode->val[0];
				mNode->val[0] = val;
			}
			else if (mNode->val[1] > val) {
				mNode->val[2] = mNode->val[1];
				mNode->val[1] = val;
			}
			else {
				mNode->val[2] = val;
			}
		}
		/* Split and Propagate (4-node) */
		else {
			temp = makeNode();
			temp->val[0] = val;
			mNode = split(mNode, temp);
			temp = NULL;
			propagate(mNode);
			mNode = NULL;
		}
	}
	/* If mNode is not a leaf, insert a value to the child node recursively */
	else if (mNode->val[0] > val) {
		insert(mNode->cNode[0], val);
	}
	else if (mNode->val[1] > val || mNode->val[1] == -1) {
		insert(mNode->cNode[1], val);
	}
	else if (mNode->val[2] > val || mNode->val[2] == -1) {
		insert(mNode->cNode[2], val);
	}
	else {
		insert(mNode->cNode[3], val);
	}
	return;

}

/************************************************************/
/* <<delete>>	Delete a value								*/
/*	Input: (node *) Delete location, (int) Deleted value	*/
/*	Output: NONE											*/
/************************************************************/
void delete(node *mNode, int val) {
	int i;
	int index;
	node *iNode;	// Interesting node

	index = search(mNode, val);
	iNode = nodeA;	// location of value
	nodeA = NULL;

	/* not exits value */
	if (index == -1)
		return;

	/* switch value with successor */
	index = putSuccessor(iNode, index);
	iNode = nodeA;	// location of predecessor
	nodeA = NULL;

	/* Delete and Relocate value (3-node and 4-node) */
	if(iNode->val[1] != -1) {
		for (i = index; i < 3; i++) {
			iNode->val[i] = iNode->val[i + 1];
		}
		iNode->val[2] = -1;
		return;
	}
	/* Delete value and reconstruct the tree (2-node) */
	else {
		iNode->val[0] = -1;
		reconst(iNode);
		return;
	}

	return;
}

/************************************************************/
/* <<search>>	Search value and find location				*/
/*	Input: (node *) searching node, (int) value				*/
/*	Output: (-1) not exist, (0~2) index of value			*/
/*			Set nodeA point a location of value				*/
/************************************************************/
int search(node *mNode, int val) {
	int i;

	/* value is in mNode */
	for (i = 0; i < 3; i++) {
		if (mNode->val[i] == val) {
			nodeA = mNode;
			return i;
		}
	}

	/* recursively call search fucntion or return -1 */
	if (mNode->val[0] > val) {
		if (mNode->cNode[0] == NULL)
			return -1;
		return search(mNode->cNode[0], val);
	}
	if (mNode->val[1] > val || mNode->val[1] == -1) {
		if (mNode->cNode[1] == NULL)
			return -1;
		return search(mNode->cNode[1], val);
	}
	if (mNode->val[2] > val || mNode->val[2] == -1) {
		if (mNode->cNode[2] == NULL)
			return -1;
		return search(mNode->cNode[2], val);
	}
	else {
		if (mNode->cNode[3] == NULL)
			return -1;
		return search(mNode->cNode[3], val);
	}
}

/************************************************************/
/* <<split>>	Combine 4-node with 2-node. Split 5-node.	*/
/*				Create 2-node with 3rd value				*/
/*	Input: (node *) 4-node, (node *) 2-node					*/
/*	Output: (node *) 2-node with 3rd value					*/
/************************************************************/
node* split(node *fNode, node *temp) {
	int i;
	node *mNode;
	mNode = makeNode();
	mNode->pNode = fNode->pNode;
	mNode->cNode[0] = fNode;
	mNode->cNode[1] = temp;
	fNode->pNode = mNode;
	temp->pNode = mNode;
	if(mNode->pNode != NULL)
		for (i = 0; i < 4; i++) {
			if (mNode->pNode->cNode[i] == fNode) {
				mNode->pNode->cNode[i] = fNode;
				break;
			}
		}
		

	if (fNode->val[0] > temp->val[0]) {
		// value setting
		mNode->val[0] = fNode->val[1];
		fNode->val[1] = fNode->val[0];
		fNode->val[0] = temp->val[0];
		temp->val[0] = fNode->val[2];
		fNode->val[2] = -1;

		// children node setting
		fNode->cNode[0] = temp->cNode[0];
		temp->cNode[0] = fNode->cNode[2];
		fNode->cNode[2] = fNode->cNode[1];
		fNode->cNode[1] = temp->cNode[1];
		temp->cNode[1] = fNode->cNode[3];
		fNode->cNode[3] = NULL;
	}
	else if (fNode->val[1] > temp->val[0]) {
		// value setting
		mNode->val[0] = fNode->val[1];
		fNode->val[1] = temp->val[0];
		temp->val[0] = fNode->val[2];
		fNode->val[2] = -1;

		// children node setting
		fNode->cNode[1] = temp->cNode[0];
		temp->cNode[0] = fNode->cNode[2];
		fNode->cNode[2] = temp->cNode[1];
		temp->cNode[1] = fNode->cNode[3];
		fNode->cNode[3] = NULL;
	}
	else if (fNode->val[2] > temp->val[0]) {
		//value setting
		mNode->val[0] = temp->val[0];
		temp->val[0] = fNode->val[2];
		fNode->val[2] = -1;

		//childeren node setting
		fNode->cNode[2] = temp->cNode[0];
		temp->cNode[0] = temp->cNode[1];
		temp->cNode[1] = fNode->cNode[3];
		fNode->cNode[3] = NULL;
	}
	else {
		//value setting
		mNode->val[0] = fNode->val[2];
		fNode->val[2] = -1;

		//childeren node setting
		fNode->cNode[3] = NULL;
	}
	equalizeNode(fNode);
	equalizeNode(temp);
	return mNode;
}

/************************************************************/
/* <<propagate>>	Combine 2-node with its parent node		*/
/*					check if root is changed or not			*/
/*	Input: (node *) 2-node									*/
/*	Output: NONE											*/
/************************************************************/
void propagate(node *mNode) {
	node *prNode = mNode->pNode;	// Propagated node (mNode's parent node)
	/* mNode is root */
	if (prNode == NULL) {
		root = mNode;
		height++;
		return;
	}

	/* split and propagate (parent node has 4-node) */
	if (prNode->val[2] != -1) {
		mNode = split(prNode, mNode);
		propagate(mNode);
		return;
	}

	/* Add a value and a child node */
	if (prNode->val[0] > mNode->val[0]) {
		prNode->val[2] = prNode->val[1];
		prNode->val[1] = prNode->val[0];
		prNode->val[0] = mNode->val[0];

		prNode->cNode[3] = prNode->cNode[2];
		prNode->cNode[2] = prNode->cNode[1];
		prNode->cNode[1] = mNode->cNode[1];
		prNode->cNode[0] = mNode->cNode[0];
	}
	else if (prNode->val[1] > mNode->val[0] || prNode->val[1] == -1) {
		prNode->val[2] = prNode->val[1];
		prNode->val[1] = mNode->val[0];

		prNode->cNode[3] = prNode->cNode[2];
		prNode->cNode[2] = mNode->cNode[1];
		prNode->cNode[1] = mNode->cNode[0];
	}
	else {
		prNode->val[2] = mNode->val[0];

		prNode->cNode[3] = mNode->cNode[1];
		prNode->cNode[2] = mNode->cNode[0];
	}
	equalizeNode(prNode);
	free(mNode);
	return;
}

/************************************************************/
/* <<putSuccessor>>	put a successor replacing a value			*/
/*	Input: (node *) location of value, (int) index of value */
/*	Output: (0~2) index of successor or value				*/
/************************************************************/
int putSuccessor(node *mNode, int index) {
	node *iNode = mNode->cNode[index + 1];

	/* mNode is leaf */
	if (iNode == NULL) {
		nodeA = mNode;
		return index;
	}

	while(1) {
		if (iNode->val[0] != -1) {
			if (iNode->cNode[0] == NULL) {
				mNode->val[index] = iNode->val[0];
				nodeA = iNode;
				return 0;
			}
		}
	
		iNode = iNode->cNode[0];
	}
}

/************************************************************/
/* <<reconst>>	Reconstruct a Tree							*/
/*	Input: (node *) A node which doesn't have any value		*/
/*	Output: NONE											*/
/*			Set nodA point a location of a Predecessor		*/
/************************************************************/
void reconst(node *mNode) {
	int i, index;
	node *mpNode = mNode->pNode;
	node *temp;

	/* Tree is empty */
	if (mpNode == NULL)
		return;

	/* Find index of no value node */
	for (i = 0; i < 4; i++) {
		if (mpNode->cNode[i] == mNode) {
			index = i;
			break;
		}
	}

	/* Transfer (3-node or 4-node sibling) */
	for (i = 0; i < 4; i++) {
		if (mpNode->cNode[i] == NULL)
			break;
		if (mpNode->cNode[i]->val[1] != -1) {
			if (index > i)
				transferR(mpNode, mNode);
			else
				transferL(mpNode, mNode);
			return;
		}
	}

	/* Fusion (3-node or 4-node parent) */
	if (mpNode->val[1] != -1) {
		fusion(mpNode, mNode);
		return;
	}

	/* Propagate (2-node sibling and 2-node parent) */
	if (index == 0) {
		mpNode->cNode[0] = mNode->cNode[0];
		if(mpNode->cNode[0] != NULL)
			mpNode->cNode[0]->pNode = mpNode;
		propagate(mpNode->cNode[1]);
	}
	else {
		mpNode->cNode[1] = mNode->cNode[0];
		if (mpNode->cNode[1] != NULL)
			mpNode->cNode[1]->pNode = mpNode;
		propagate(mpNode->cNode[0]);
	}
	free(mNode);

	/** root **/
	if (mpNode->pNode == NULL) {
		height--;
		return;
	}

	/** recursively recunstruct the tree **/
	temp = makeNode();
	temp->pNode = mpNode->pNode;
	temp->cNode[0] = mpNode;
	mpNode->pNode = temp;
	if(temp->pNode != NULL)
		for (i = 0; i < 4; i++) {
			if (temp->pNode->cNode[i] == mpNode) {
				temp->pNode->cNode[i] = temp;
				break;
			}
		}
	reconst(temp);
	return;
}

/************************************************************/
/* <<transferR>>	Transfer a value to the right sibling	*/
/*	Input: (node *) pNode of receiver, (node *) receiver	*/
/*	Output: NONE											*/
/************************************************************/
void transferR(node *mNode, node *receiver) {
	int i, num;
	node *giver;
	
	for (i = 1; i < 4; i++) {
		if (mNode->cNode[i] == receiver) {
			
			giver = mNode->cNode[i - 1];
			num = findNum(giver);		// # of sibling node's value

			/* value change */
			receiver->val[0] = mNode->val[i - 1];
			mNode->val[i - 1] = giver->val[num - 1];
			giver->val[num - 1] = -1;

			/* cNode change */
			receiver->cNode[1] = receiver->cNode[0];
			receiver->cNode[0] = giver->cNode[num];
			giver->cNode[num] = NULL;

			if(receiver->cNode[0] != NULL)
				receiver->cNode[0]->pNode = receiver;

			/* recursively transfer */
			if (num == 1) 
				transferR(mNode, giver);

			return;
		}
	}
}

/************************************************************/
/* <<transferL>>	Transfer a value to the left sibling	*/
/*	Input: (node *) pNode of receiver, (node *) receiver	*/
/*	Output: NONE											*/
/************************************************************/
void transferL(node *mNode, node *receiver) {
	int i, num;
	node *giver;

	for (i = 3; i >= 0; i--) {
		if (mNode->cNode[i] == receiver) {

			giver = mNode->cNode[i + 1];
			num = findNum(giver);		// # of sibling node's value

			/* value change */
			receiver->val[0] = mNode->val[i];
			mNode->val[i] = giver->val[0];
			giver->val[0] = giver->val[1];
			giver->val[1] = giver->val[2];
			giver->val[2] = -1;

			/* cNode change */
			receiver->cNode[1] = giver->cNode[0];
			giver->cNode[0] = giver->cNode[1];
			giver->cNode[1] = giver->cNode[2];
			giver->cNode[2] = giver->cNode[3];
			giver->cNode[3] = NULL;

			if (receiver->cNode[1] != NULL)
				receiver->cNode[1]->pNode = receiver;

			/* recursively transfer */
			if (num == 1) 
				transferL(mNode, giver);

			return;
		}
	}
}

/************************************************************/
/* <<fusion>>	Delete a node, Fusion a sibling and a parent*/
/*	Input: (node *) pNode of dNode, (node *) deleted node	*/
/*	Output: NONE											*/
/************************************************************/
void fusion(node *mNode, node *dNode) {
	int i, j;

	/* Fusion with Right sibling */
	if (mNode->cNode[0] == dNode) {
		/* node lotate */
		mNode->cNode[0] = mNode->cNode[1];
		mNode->cNode[1] = mNode->cNode[2];
		mNode->cNode[2] = mNode->cNode[3];
		mNode->cNode[3] = NULL;

		/* value change */
		mNode->cNode[0]->val[1] = mNode->cNode[0]->val[0];
		mNode->cNode[0]->val[0] = mNode->val[0];
		mNode->val[0] = mNode->val[1];
		mNode->val[1] = mNode->val[2];
		mNode->val[2] = -1;

		/* node change */
		mNode->cNode[0]->cNode[2] = mNode->cNode[0]->cNode[1];
		mNode->cNode[0]->cNode[1] = mNode->cNode[0]->cNode[0];
		mNode->cNode[0]->cNode[0] = dNode->cNode[0];
		if(dNode->cNode[0] != NULL)
			mNode->cNode[0]->cNode[0]->pNode = mNode->cNode[0];
	}
	/* Fusion with Left sibling */
	else {
		for (i = 1; i < 4; i++) {
			if (mNode->cNode[i] != dNode)
				continue;

			/* node lotate */
			for (j = i; j < 3; j++) {
				mNode->cNode[j] = mNode->cNode[j + 1];
			}
			mNode->cNode[3] = NULL;

			/* value change */
			mNode->cNode[i - 1]->val[1] = mNode->val[i - 1];
			for (j = i - 1; j < 2; j++) {
				mNode->val[j] = mNode->val[j + 1];
			}
			mNode->val[2] = -1;

			/* node change*/
			mNode->cNode[i - 1]->cNode[2] = dNode->cNode[0];
			if (mNode->cNode[i - 1]->cNode[2] != NULL)
				mNode->cNode[i - 1]->cNode[2]->pNode = mNode->cNode[i - 1];
			break;
		}
	}

	free(dNode);
	return;
}

/************************************************************/
/* <<equalizeNode>>	Make children point new parent			*/
/*	Input: (node *) parent node								*/
/*	Output: NONE											*/
/************************************************************/
void equalizeNode(node *mNode) {
	int i;
	for (i = 0; i < 4; i++) {
		if (mNode->cNode[i] == NULL)
			break;
		mNode->cNode[i]->pNode = mNode;
	}
	return;
}

/************************************************************/
/* <<findNum>>	Find the number of value in a node			*/
/*	Input: (node *) node									*/
/*	Output: (int) the number of value in the node			*/
/************************************************************/
int findNum(node *mNode) {
	int count = 0;

	if (mNode->val[2] != -1)
		return 3;
	if (mNode->val[1] != -1)
		return 2;
	if (mNode->val[0] != -1)
		return 1;

	return 0;
}

/************************************************************/
/* <<printNode>>	Print a node on the screen				*/
/*	Input: (node *) node									*/
/*	Output: NONE											*/
/************************************************************/
void printNode(node *mNode) {
	if (mNode->val[0] == -1)
		return;
	else if (mNode->val[1] == -1)
		printf("(%d) ", mNode->val[0]);
	else if (mNode->val[2] == -1)
		printf("(%d %d) ", mNode->val[0], mNode->val[1]);
	else
		printf("(%d %d %d) ", mNode->val[0], mNode->val[1], mNode->val[2]);

	return;
}

/************************************************************/
/* <<printTree>>	Print a Tree on the screen				*/
/*	Input: (node *) root									*/
/*	Output: NONE											*/
/************************************************************/
void printTree(node *mNode) {
	int count;	// the number of nodes in a row
	node *nodeA[maxLength];	// Array1 of nodes in one row
	node *nodeB[maxLength]; // Array2 of nodes in one row
	memset(nodeA, 0, sizeof(nodeA));	// Initialize nodeA
	memset(nodeB, 0, sizeof(nodeB));	// Initialize nodeB
	nodeA[0] = mNode;	// Put a root in nodeA
	int i, j, k;

	/* Main Loop */
	for (i = 0; i < height; i++) {
		count = 0;
		if (i % 2 == 0) {
			for (j = 0; j < maxLength; j++) {
				if (nodeA[j] == NULL) {
					break;
				}
				/* print a node */
				printNode(nodeA[j]);
				/* put child nodes in another array and count */
				for (k = 0; k < 4; k++) {
					if (nodeA[j]->cNode[k] == NULL)
						break;
					nodeB[count] = nodeA[j]->cNode[k];
					count++;
				}
			}
		}
		else {
			for (j = 0; j < maxLength; j++) {
				if (nodeB[j] == NULL)
					break;
				/* print a node */
				printNode(nodeB[j]);
				/* put child nodes in another array and count */
				for (k = 0; k < 4; k++) {
					if (nodeB[j]->cNode[k] == NULL)
						break;
					nodeA[count] = nodeB[j]->cNode[k];
					count++;
				}
			}
		}
		printf("\n");
	}
	return;
}

/************************************************************/
/* <<fprintNode>>	Write a node on the "output.txt"		*/
/*					printNode for Batch mode				*/
/*	Input: (node *) node									*/
/*	Output: NONE											*/
/************************************************************/
void fprintNode(node *mNode) {
	if (mNode->val[0] == -1)
		return;
	else if (mNode->val[1] == -1)
		fprintf(output, "(%d) ", mNode->val[0]);
	else if (mNode->val[2] == -1)
		fprintf(output, "(%d %d) ", mNode->val[0], mNode->val[1]);
	else
		fprintf(output, "(%d %d %d) ", mNode->val[0], mNode->val[1], mNode->val[2]);

	return;
}

/************************************************************/
/* <<fprintTree>>	Write a Tree on the "output.txt"		*/
/*					printTree for Batch mode				*/
/*	Input: (node *) root									*/
/*	Output: NONE											*/
/************************************************************/
void fprintTree(node *mNode) {
	int count;	// the number of nodes in a row
	node *nodeA[maxLength];	// Array1 of nodes in one row
	node *nodeB[maxLength]; // Array2 of nodes in one row
	memset(nodeA, 0, sizeof(nodeA));	// memory allocate for nodeA
	memset(nodeB, 0, sizeof(nodeB));	// memory allocate for nodeB
	nodeA[0] = mNode;	// Put a root in nodeA
	int i, j, k;
	for (i = 0; i < height; i++) {
		count = 0;
		if (i % 2 == 0) {
			for (j = 0; j < maxLength; j++) {
				if (nodeA[j] == NULL) {
					break;
				}
				/* write a node */
				fprintNode(nodeA[j]);
				/* put child nodes in another array and count */
				for (k = 0; k < 4; k++) {
					if (nodeA[j]->cNode[k] == NULL)
						break;
					nodeB[count] = nodeA[j]->cNode[k];
					count++;
				}
			}
		}
		else {
			for (j = 0; j < maxLength; j++) {
				if (nodeB[j] == NULL)
					break;
				/* write a node */
				fprintNode(nodeB[j]);
				/* put child nodes in another array and count */
				for (k = 0; k < 4; k++) {
					if (nodeB[j]->cNode[k] == NULL)
						break;
					nodeA[count] = nodeB[j]->cNode[k];
					count++;
				}
			}
		}
		fprintf(output, "\n");
	}
	return;
}