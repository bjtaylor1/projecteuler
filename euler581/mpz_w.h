#pragma once

#ifndef CLASS_MPZ_W
#define CLASS_MPZ_W

class mpz_w
{
public:
	static mpz_w one;
	mpz_t val;
	mpz_w();
	mpz_w(long long ui);
	mpz_w& operator++();
	mpz_w& operator/=(const mpz_w&);
	mpz_w& operator+=(const mpz_w&);
	bool is_even() const;
	mpz_w sqrt() const;
};

mpz_w operator+(const mpz_w&, const mpz_w&);
mpz_w operator-(const mpz_w&, const mpz_w&);
mpz_w operator*(const mpz_w&, const mpz_w&);
mpz_w operator/(const mpz_w&, const mpz_w&);
mpz_w operator%(const mpz_w&, const mpz_w&);
bool operator<=(const mpz_w&, long long);
bool operator<=(const mpz_w&, const mpz_w&);
bool operator>(const mpz_w&, const mpz_w&);
bool operator!=(const mpz_w&, const mpz_w&);
bool operator==(const mpz_w&, const mpz_w&);

#endif
