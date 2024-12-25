#include <stdlib.h>
#include "bigint.h"

static void print(bigint* N);

/*############################# code ################################*/

/*from 2019*/
static bigint* bigint_alloc(digit x) {
	bigint* tmp = (bigint*)malloc(sizeof(bigint));

	if (tmp != NULL) {
		tmp->x = x;
		tmp->next = NULL;
		tmp->prev = NULL;
	}
	return tmp;
}

/*from 2019*/
static int head_insert(bigint** N, digit x) {
	if (N == NULL) return 1;
	else if (*N == NULL) {
		return (*N = bigint_alloc(x)) == NULL;
	}
	else {
		bigint* tmp = bigint_alloc(x);

		if (tmp != NULL) {
			tmp->next = *N;
			(*N)->prev = tmp;
			*N = tmp;
		}
		return tmp == NULL;
	}
}

/*works - does not move pointer*/
static int handle_carry(bigint* lsb, bigint** msb) {
	bigint* node = lsb;

	/*redundant set in case of not entering loop*/
	unsigned int c = node->x / 10;

	while (node->prev != NULL) {
		
		/*if node->x < 9 then c = 0*/
		c = node->x / 10;
		node->x %= 10;
		
		node = node->prev;
		node->x += c;
	}
	if (node->x > 9) {
		c = node->x / 10;
		node->x %= 10;
		if (head_insert(&node, c) == 1) return 1;
		//msb = msb->prev;
		*msb = node;
	}
	/*print(node);
	print(msb);*/
	print(*msb);
	
	return 0;
}

/*works*/
static void add_zeroes(bigint* n, unsigned int zeroes) {
	
	/*get last node*/
	bigint* last_node = n;
	while (last_node->next != NULL) {
		last_node = last_node->next;
	}

	/*create dl list of zeroes*/
	int i;
	bigint* z_head = NULL;
	for (i = 0; i < zeroes; i++) {
		head_insert(&z_head, 0);
	}

	/*merge lists*/
	last_node->next = z_head;
	if (z_head != NULL) z_head->prev = last_node;
}

/*works - does not move poiter*/
static bigint* digit_mult(bigint* lsb, unsigned int k) {
	
	bigint* node = lsb, *res = bigint_alloc(node->x * k), *res_lsb = res;

	node = node->prev;
	while (node != NULL) {
		head_insert(&res, node->x * k);
		node = node->prev;
	}
	
	if (handle_carry(res_lsb, &res)) printf("\ndigit mult error");
	//print(res);
	return res;
}

/*yea it works dw*/
static bigint* bigint_sum(bigint* lsb1, bigint* lsb2) {
	
	bigint* s1 = lsb1, * s2 = lsb2;
	while (s1->next != NULL) s1 = s1->next;
	while (s2->next != NULL) s2 = s2->next;

	bigint* res = bigint_alloc(s1->x + s2->x), * res_lsb = res;

	s1 = s1->prev;
	s2 = s2->prev;
	while (s1 != NULL || s2 != NULL) {
		head_insert(&res,
			(s1 != NULL ? s1->x : 0) +
			(s2 != NULL ? s2->x : 0)
		);
		if (s1 != NULL) s1 = s1->prev;
		if (s2 != NULL) s2 = s2->prev;
	}
	if (handle_carry(res_lsb, &res)) printf("\nsum error");
	print(res);
	return res;
}

/**/
bigint *mul(bigint *N1, bigint *N2) {
	bigint *N = NULL;

	/*presumendo N1 e N2 siano validi*/ 
	if (N1 == NULL || N2 == NULL) {
		return NULL;
	}

	/*setup*/
	bigint* n = N2, *r = N = bigint_alloc(0), *d = N1;
	if (N == NULL) return NULL; 
	
	/*gestisco segno*/
	int sign = (n->x * d->x < 0 ? -1 : 1);
	if (n->x < 0) n->x = -n->x;
	if (d->x < 0) d->x = -d->x;
	
	/*get len*/
	unsigned int len = 0;
	while (n->next != NULL) {
		n = n->next;
		len++;
	}

	while (d->next != NULL) {
		d = d->next;
	}

	/*note: n is LSB of N2, d is LSB of N1, r is LSB of N*/
	unsigned int i;
	for (i = 0; i <= len; i++)
	{
		bigint* res = digit_mult(d, n->x);
		n = n->prev;
		add_zeroes(res, i);
		//printf("res: ");  print(res);

		N = bigint_sum(N, res);
		//printf("N: ");  print(N);
	}

	N->x *= sign;
	return N;
}

/*############################# testing ################################*/

static void print(bigint* N) {
	if (N == NULL) {
		printf("NaN");
	}
	else {
		while (N != NULL) {
			printf("%d -> ", N->x);
			N = N->next;
		}
	}
	printf("\n");
}

int main(int argc, char* argv[]) {
	bigint* N1, * N2, * N;

	N1 = bigint_alloc(9);
	N2 = bigint_alloc(3);
	head_insert(&N2, 2);
	head_insert(&N2, 7);
	head_insert(&N2, 9);
	head_insert(&N2, 8);
	head_insert(&N2, 6);
	head_insert(&N2, 5);
	head_insert(&N2, 4);
	head_insert(&N2, 1);
	N = mul(N1, N2);

	/*print(N1);
	print(N2)*/;
	printf("\n");
	print(N);

	return 0;
}
