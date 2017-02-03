#include "stdafx.h"
#include "mpz_w.h"

mpz_w mpz_w::one(1);

mpz_w::mpz_w()
{
	mpz_init(val);
}

mpz_w::mpz_w(long long ui)
{
	mpz_init_set_ui(val, ui);
}

mpz_w& mpz_w::operator++()
{
	mpz_add(val, val, mpz_w::one.val);
	return *this;
}

mpz_w & mpz_w::operator/=(const mpz_w& rhs)
{
	mpz_fdiv_q(val, val, rhs.val);
	return *this;
}

mpz_w & mpz_w::operator+=(const mpz_w& rhs)
{
	mpz_add(val, val, rhs.val);
	return *this;
}

bool mpz_w::is_even() const
{
	return mpz_even_p(val);
}

mpz_w mpz_w::sqrt() const
{
	mpz_w result;
	mpz_sqrt(result.val, val);
	return result;
}

mpz_w operator+(const mpz_w& lhs, const mpz_w& rhs)
{
	mpz_w result;
	mpz_add(result.val, lhs.val, rhs.val);
	return result;
}

mpz_w operator-(const mpz_w& lhs, const mpz_w& rhs)
{
	mpz_w result;
	mpz_sub(result.val, lhs.val, rhs.val);
	return result;
}

mpz_w operator*(const mpz_w& lhs, const mpz_w& rhs)
{
	mpz_w result;
	mpz_mul(result.val, lhs.val, rhs.val);
	return result;
}

mpz_w operator/(const mpz_w& lhs, const mpz_w& rhs)
{
	mpz_w result;
	mpz_fdiv_q(result.val, lhs.val, rhs.val);
	return result;
}

mpz_w operator%(const mpz_w& n, const mpz_w& d)
{
	mpz_w result;
	mpz_mod(result.val, n.val, d.val);
	return result;
}

bool operator<=(const mpz_w& n, long long c)
{
	return mpz_cmp_ui(n.val, c) <= 0;
}

bool operator<=(const mpz_w& n, const mpz_w& c)
{
	return mpz_cmp(n.val, c.val) <= 0;
}

bool operator>(const mpz_w& n, const mpz_w& c)
{
	return mpz_cmp(n.val, c.val) > 0;
}

bool operator!=(const mpz_w& lhs, const mpz_w& rhs)
{
	return mpz_cmp(lhs.val, rhs.val) != 0;
}

bool operator==(const mpz_w& lhs, const mpz_w& rhs)
{
	return mpz_cmp(lhs.val, rhs.val) == 0;
}


mpz_w operator++(mpz_w& n)
{
	return n = n + mpz_w::one;
}
mpz_w operator++(const mpz_w& n)
{
	return n + mpz_w::one;
}
