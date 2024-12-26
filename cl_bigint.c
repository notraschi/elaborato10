#include <stdlib.h>
#include "bigint.h"

/*from mul 2019*/
static bigint* bigint_alloc(digit x) {
	bigint* tmp = (bigint*)malloc(sizeof(bigint));

	if (tmp != NULL) {
		tmp->x = x;
		tmp->next = tmp;
		tmp->prev = tmp;
	}
	return tmp;
}

/*from mul 2019*/
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


/*from mul 2019*/
static int bigint_insert(bigint* N, digit x) {
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

/*from mul 2019*/
static int head_insert(bigint** N, digit x) {
	if (N == NULL)
		return 1;
	else if (*N == NULL)
		return (*N = bigint_alloc(x)) == NULL;
	else if (bigint_insert((*N)->prev, x) == 1)
		return 1;
	else
		return (*N = (*N)->prev) == NULL;
}

/*from mul 2019*/
static int head_delete(bigint** N) {
	if (N == NULL || *N == NULL) {
		return 1;
	}
	else if (*N == (*N)->next) {
		free(*N);
		*N = NULL;
		return 0;
	}
	else {
		*N = (*N)->next;
		return bigint_delete((*N)->prev);
	}
}

/*from mul 2019*/
static void remove_leading_zeros(bigint** N) {
	if (N != NULL && *N != NULL) {
		while ((*N)->x == 0 && *N != (*N)->next)
			head_delete(N);
	}
}

/*works*/
static int handle_carry(bigint* lsb, bigint** msb) {
	bigint* node = lsb;

	/*redundant set in case of not entering loop*/
	unsigned int c = node->x / 10;

	while (node->prev != lsb) {

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
		*msb = node;
	}
	/*print(*msb);*/
	return 0;
}

static void add_zeroes(bigint* n, unsigned int zeroes) {

	/*get last node*/
	bigint* last_node = n->prev;

	/*create dl list of zeroes*/
	int i;
	for (i = 0; i < zeroes; i++) {
		bigint_insert(last_node, 0);
	}
}

static bigint* digit_mult(bigint* lsb, unsigned int k) {

	bigint* node = lsb, * res = bigint_alloc(node->x * k), * res_lsb = res;

	node = node->prev;
	while (node != lsb) {
		head_insert(&res, node->x * k);
		node = node->prev;
	}

	if (handle_carry(res_lsb, &res)) printf("\ndigit mult error");
	/*print(res);*/
	return res;
}


static bigint* bigint_sum(bigint* lsb1, bigint* lsb2) {

	bigint* s1 = lsb1, * s2 = lsb2;
	/*while (s1->next != NULL) s1 = s1->next;
	while (s2->next != NULL) s2 = s2->next;*/ /*gli do cifra meno significativa io :)*/

	bigint* res = bigint_alloc(s1->x + s2->x), * res_lsb = res;

	s1 = s1->prev;
	s2 = s2->prev;
	while (s1 != lsb1 || s2 != lsb2) {
		head_insert(&res,
			(s1 != lsb1 ? s1->x : 0) +
			(s2 != lsb2 ? s2->x : 0)
		);
		if (s1 != lsb1) s1 = s1->prev;
		if (s2 != lsb2) s2 = s2->prev;
	}
	if (handle_carry(res_lsb, &res)) printf("\nsum error");
	/*print(res);*/
	return res;
}


bigint *mul(bigint *N1, bigint *N2) {
	bigint *N = NULL;

	/*presumendo N1 e N2 siano validi*/
	if (N1 == NULL || N2 == NULL) {
		return NULL;
	}

	/*setup*/
	bigint* n = N2, * d = N1;
	N = bigint_alloc(0);
	if (N == NULL) return NULL;

	/*gestisco segno*/
	int sign = (n->x * d->x < 0 ? -1 : 1);
	if (n->x < 0) n->x = -n->x;
	if (d->x < 0) d->x = -d->x;

	/*get len*/
	unsigned int len = 0;
	while (n->next != N2) {
		n = n->next;
		len++;
	}

	/*note: n is LSB of N2, d->prev is LSB of N1*/
	unsigned int i;
	for (i = 0; i <= len; i++)
	{
		bigint* res = digit_mult(d->prev, n->x);
		n = n->prev;
		add_zeroes(res, i);

		N = bigint_sum(N->prev, res->prev);
	}

	N->x *= sign;
	remove_leading_zeros(&N);
	return N;

}
