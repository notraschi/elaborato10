#include <stdlib.h>
#include "bigint.h"

static void print(bigint* N);

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
static int handle_carry(bigint* lsb) {
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
		if (head_insert(&node, c) == 1) return 1;
	}
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
	
	if (handle_carry(res_lsb)) printf("\nerror");
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

	/*note: n is lsb of N2, d is MSB of N1*/
	

	N = digit_mult(n, d->x);
	N->x *= sign;
	add_zeroes(N, 3);
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

	N1 = bigint_alloc(2);
	N2 = bigint_alloc(4);
	N = mul(N1, N2);

	print(N);

	return 0;
}
