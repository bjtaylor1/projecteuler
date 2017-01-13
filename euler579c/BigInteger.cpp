/*
 * BigInteger Class, performs basic arithmetic operations of very large integers.
 * Copyright (C) 2011  Mahmoud Mechehoul
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "stdafx.h"
#include "BigInteger.h"

using namespace std;

BigInteger::BigInteger(unsigned long long integer) : totalTruncated(0) {
	setInteger(integer);
}

BigInteger::BigInteger(string integer): totalTruncated(0) {
	for (long long i = 0; i < (long long)integer.size(); i++) {
		if (!(integer[i] >= '0' && integer[i] <= '9')) throw runtime_error("Bad character in integer representation");
		this->integer += integer[i];
	}

	if (this->integer.size() == 0) {
		this->integer = "0";
	} else {
		this->integer = integer.substr(getTrimIndex(integer));
	}
}

void BigInteger::setInteger(unsigned long long integer) {
	if (integer == 0) this->integer = "0";

	while (integer) {
		this->integer = (char)((integer % 10) + '0') + this->integer;
		integer /= 10;
	}
}

void BigInteger::setInteger(string integer) {
	this->integer = integer;
}

unsigned long long BigInteger::getIntValue() const {
	unsigned long long ret = 0;
	unsigned long long biggest = 0xFFFFFFFF;
	for (long long i = 0; i < (long long)integer.size(); i++) {
		long long unit = integer[i] - '0';
		if (ret > (biggest - unit) / 10.0) return 0;
		ret = ret * 10 + unit;
	}
	return ret;
}

string BigInteger::toString() const {
	return integer;
}

void BigInteger::truncate(long n) {
	if (integer.size() > n) {
		long toTruncate = integer.size() - n;
		totalTruncated += toTruncate;
		integer.erase(0, toTruncate);
	}
}

BigInteger BigInteger::addInteger(const BigInteger& integer_to_add) const {
	long long a_n = max((long long)(integer_to_add.toString().size() - toString().size()), 0LL);
	long long b_n = max((long long)(toString().size() - integer_to_add.toString().size()), 0LL);
	string a = string(a_n, '0') + toString();
	string b = string(b_n, '0') + integer_to_add.toString();

	reverse(a.begin(), a.end());
	reverse(b.begin(), b.end());

	string result; long long carry = 0;

	for (long long i = 0; i < (long long)a.size(); i++) {
	   long long sum = (a[i] - '0') + (b[i] - '0') + carry;
	   result += ((char)(sum % 10 + '0'));
	   carry = sum / 10;
	}

	if (carry != 0) result += ((char)(carry + '0'));

	reverse(result.begin(), result.end());

	return BigInteger(result.substr(getTrimIndex(result)));
}

BigInteger BigInteger::addInteger(const string& integer_to_add) const {
	return addInteger(BigInteger(integer_to_add));
}

BigInteger BigInteger::multiplyInteger(const BigInteger& integer_to_multiply) const {
	string a = integer_to_multiply.toString();
	string b = toString();

	reverse(a.begin(), a.end());
	reverse(b.begin(), b.end());

	BigInteger ret("0");

	for (long long i = 0; i < (long long)a.size(); i++) {
		long long carry = 0; string tmp = string(i, '0');

		for (long long j = 0; j < (long long)b.size(); j++) {
			long long mul = (a[i] - '0') * (b[j] - '0') + carry;
			tmp += ((char)(mul % 10 + '0'));
			carry = mul / 10;
		}

		if (carry != 0) tmp += (carry + '0');

		reverse(tmp.begin(), tmp.end());

		ret = ret.addInteger(tmp.substr(getTrimIndex(tmp)));
	}

	return ret;
}

BigInteger BigInteger::multiplyInteger(const string& integer_to_multiply) const {
	return multiplyInteger(BigInteger(integer_to_multiply));
}

size_t BigInteger::getTrimIndex(const string& integer) {
	size_t index = 0;
	while (integer[index] == '0' && index < integer.size() - 1) index++;
	return index;
}

bool BigInteger::operator==(const BigInteger& integer) const {
	return this->integer == integer.toString();
}

BigInteger BigInteger::operator+(const BigInteger& integer) const {
	return addInteger(integer);
}

BigInteger BigInteger::operator*(const BigInteger& integer) const {
	return multiplyInteger(integer);
}

ostream& operator<<(ostream& in, BigInteger& integer) {
	
	in << integer.toString();
	if (integer.totalTruncated > 0)
		in << " (e" << integer.totalTruncated << ")";

	return in;
}
