// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
    if (len <= 0)
        throw "Non equal size";

     BitLen = len;
     MemLen = (BitLen + sizeof(TELEM) * 8 - 1) / (sizeof(TELEM) * 8); // Вычисляем количество элементов для хранения битов
     pMem = new TELEM[MemLen];

     for (int i = 0; i < MemLen; i++)
         pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
    delete []pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if (n < 0 || n >= BitLen)
        throw "Non equal size";

    return (n / (sizeof(TELEM) * 8)); // Индекс элемента в массиве pMem
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    TELEM res;
    res = TELEM(1) << (sizeof(TELEM) * 8 - n - 1);

    return res;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n >= BitLen)
        throw "Non equal size";

    long long nVar = n / (sizeof(TELEM) * 8);
    long long nBit = n % (sizeof(TELEM) * 8);
    pMem[nVar] = pMem[nVar] | GetMemMask(nBit);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n >= BitLen)
        throw "Non equal size";

    int nVar = n / (sizeof(TELEM) * 8);
    int nBit = n % (sizeof(TELEM) * 8);
    pMem[nVar] = pMem[nVar] & ~GetMemMask(nBit);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n >= BitLen)
        throw "Non equal size";

    int nVar = n / (sizeof(TELEM) * 8);
    int nBit = n % (sizeof(TELEM) * 8);
    TELEM res = pMem[nVar];
    res = (res & GetMemMask(n)) >> ((sizeof(TELEM) * 8) - n - 1);

    return res;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    delete[] pMem;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
    {
        pMem[i] = bf.pMem[i];
    }
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    int temp = 1;
    if (BitLen != bf.BitLen)
    {
        return 0;
    }
    for (int i = 0; i < MemLen; i++)
    {
        if(pMem[i] == bf.pMem[i]){}
        else
        {
            temp = 0;
        }
    }
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    int temp = 0;
    if (BitLen != bf.BitLen)
    {
        return 1;
    }
    for (int i = 0; i < MemLen; i++)
    {
        if (pMem[i] == bf.pMem[i]){}
        else
        {
            temp = 1;
        }
    }
    return temp;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    TBitField res = TBitField(std::max(BitLen, bf.BitLen));

    for (int i = 0; i < std::min(MemLen, bf.MemLen); i++)
    {
        res.pMem[i] = pMem[i] | bf.pMem[i];
    }

    if (MemLen > bf.MemLen)
    {
        for (int i = std::min(MemLen, bf.MemLen); i < std::max(MemLen, bf.MemLen); i++)
        {
            res.pMem[i] = pMem[i];
        }
    }
    else
    {
        for (int i = std::min(MemLen, bf.MemLen); i < std::max(MemLen, bf.MemLen); i++)
            res.pMem[i] = bf.pMem[i];
    }



    return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    TBitField res = TBitField(std::max(BitLen, bf.BitLen));
    for (int i = 0; i < std::min(MemLen, bf.MemLen); i++)
    {
        res.pMem[i] = pMem[i] & bf.pMem[i];
    }
    return res;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField result(BitLen);
    for (int i = 0; i < MemLen; i++)
        result.pMem[i] = ~pMem[i];

    // Убираем лишние биты (если длина BitLen не кратна размеру TELEM * 8)
    int lastBits = BitLen % (sizeof(TELEM) * 8);
    if (lastBits)
    {
        TELEM mask = (1 << lastBits) - 1;
        result.pMem[MemLen - 1] &= mask;
    }

    return result;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    char ch;
    for (int i = 0; i < bf.BitLen; i++)
    {
        istr >> ch;
        if (ch == '1')
            bf.SetBit(i);
        else if (ch == '0')
            bf.ClrBit(i);
        else
            throw std::invalid_argument("Invalid character in bit field input");
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.BitLen; i++)
    {
        ostr << bf.GetBit(i);
    }
    return ostr;
}
