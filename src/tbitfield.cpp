// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len < 0) throw "NEGATIVE_LENGTH";
	BitLen = len;
	MemLen = (len+31) >> 5;
	pMem = new TELEM [MemLen];
	if (pMem != 0)
		for (int i = 0; i < MemLen; i++)
			pMem[i] = 0;
	
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM [MemLen];
	if (pMem != 0)
		for (int i = 0; i < MemLen; i++)
			pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
	delete [] pMem;
	pMem = NULL;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	return n >> 5;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	TELEM tmp = 1;
	return tmp << (n % 32);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if ((n > -1)&&(n < BitLen))
	{
		int i = GetMemIndex(n);
		pMem[i] = pMem[i] | GetMemMask(n);
	}
	else throw "SET_BIT_WITH_NEGATIVE_OR_TOO_LARGE_INDEX";
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if ((n > -1)&&(n < BitLen))
	{
		int i = GetMemIndex(n);
		pMem[i] = pMem[i] & ~GetMemMask(n);
	}
	else throw "CLEAR_BIT_WITH_NEGATIVE_OR_TOO_LARGE_INDEX";
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if ((n > -1)&&(n < BitLen))
	{
		int i = GetMemIndex(n);
		return (pMem[i] & GetMemMask(n)) >> (n % 16);
	}
	else throw "GET_BIT_WITH_NEGATIVE_OR_TOO_LARGE_INDEX";
	return 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (this != &bf)
	{
		BitLen = bf.BitLen;
		if (MemLen != bf.MemLen) 
		{
			MemLen = bf.MemLen;
			if (pMem != NULL) delete []pMem;
			pMem = new TELEM [MemLen];
		}
		
		if (pMem != NULL)
			for (int i = 0; i < MemLen; i++)
			{
				pMem[i] = bf.pMem[i];
			}
	}
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	TELEM MemMask = 0xffffffff;
	int result = 1;
	if (BitLen != bf.BitLen) 
		result = 0;
	else
		for (int i = 0; i < MemLen-1; i++)
			if ((pMem[i] & MemMask) != (bf.pMem[i] & MemMask))
			{
				result = 0;
				break;
			}
		for (int i = (MemLen-1) << 5; i < BitLen; i++)
			if (GetBit(i) != bf.GetBit(i))
			{
				result = 0;
				break;
			}
	return result;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	int result = 0;
	if (BitLen != bf.BitLen) 
		result = 1;
	else
		for (int i = 0; i < MemLen; i++)
			if (pMem[i] != bf.pMem[i])
			{
				result = 1;
				break;
			}
	return result;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	int len = BitLen;
	if (BitLen < bf.BitLen) 
		len = bf.BitLen;
	
	TBitField tmp(len);
	for (int i = 0; i < MemLen; i++)
		tmp.pMem[i] = pMem[i];
	for (int i = 0; i < bf.MemLen; i++)
		tmp.pMem[i] |= bf.pMem[i];
	return tmp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int len = BitLen;
	if (BitLen < bf.BitLen) 
		len = bf.BitLen;
	
	TBitField tmp(len);
	for (int i = 0; i < MemLen; i++)
		tmp.pMem[i] = pMem[i];
	for (int i = 0; i < bf.MemLen; i++)
		tmp.pMem[i] &= bf.pMem[i];
	for (int i = bf.BitLen; i < BitLen; i++)
		tmp.ClrBit(i);
	return tmp;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField tmp(BitLen);
	for (int i = 0; i < MemLen; i++)
		tmp.pMem[i] = ~pMem[i];
	return tmp;

}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	int i=0;
	char ch;
	do	{istr >> ch;}	while (ch != ' ');
	
	while (1)
	{
		istr >> ch;
		if (ch == '0') 
			bf.ClrBit(i++);
		else if  (ch == '1')
			bf.SetBit(i++);
		else break;
	}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i = 0; i < bf.GetLength(); i++)
		if (bf.GetBit(i)) ostr << '1';
		else ostr << '0';
	return ostr;
}
