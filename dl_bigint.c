#include <stdlib.h>
#include "bigint.h"

static bigint* bigint_alloc(digit x) {
	bigint* tmp = (bigint*)malloc(sizeof(bigint));

	if (tmp != NULL) {
		tmp->x = x;
		tmp->next = tmp;
		tmp->prev = tmp;
	}
	return tmp;
}

static int bigint_delete(bigint* N) {
	if (N == NULL) {
		return 1;
	}
	else {
		bigint* prv = N->prev, * nxt = N->next;
		nxt->prev = prv;
		prv->next = nxt;
		free(N);
		return 0;
	}
}

static int bigint_insert_after(bigint* N, digit x) {
	if (N == NULL) {
		return 1;
	}
	else {
		bigint* tmp = bigint_alloc(x), * nxt = N->next, * prv = N;
		if (tmp != NULL) {
			tmp->prev = prv;
			tmp->next = nxt;
			prv->next = tmp;
			nxt->prev = tmp;
		}
		return tmp == NULL;
	}
}

static int bigint_insert_head(bigint* N, digit x) {
	if (N == NULL) {
		return 1;
	}
	else {
		bigint* tmp = bigint_alloc(x);
		if (tmp != NULL) {
			tmp->prev = NULL;
			tmp->next = N;
			N->prev = tmp;
		}
		return tmp == NULL;
	}
}


/*fixes the carry in the bigint, takes pointer to LSB and keeps it there*/
static int handle_carry(bigint* tail) {
	bigint* node = tail;
	unsigned int c = node->x / 10;
	while (node->prev != NULL) {
		if (node->x > 9) {
			c = node->x / 10;
			node->x %= 10;
		} else {
			c = 0;
		}
		node = node->prev;
		node->x += c;
	}
	if (node->x > 9) {
		if (bigint_insert_head(node, c) == 1) return 1;
		printf("allocated new head");
	}
	return 0;
}

static bigint* digit_mult(bigint* n, unsigned int k) {

}

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

	
	/**/

	N->x *= sign;
	return N;
}
