// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

TSet::TSet(int mp) : BitField(mp)
{
    MaxPower = mp;
}

// конструктор копирования
TSet::TSet(const TSet &s) : BitField(s.BitField)
{
    MaxPower = s.MaxPower;
}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : BitField(bf)
{
    MaxPower = 0;
}

TSet::operator TBitField()
{
    return TBitField(0);
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества
{
    if (Elem < 0 || Elem >= MaxPower)
        throw "Non equal size";
    if (Elem >= 0 && Elem < MaxPower)
        return BitField.GetBit(Elem);  
    else
        return 0;
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    if (Elem < 0 || Elem >= MaxPower)
        throw "Non equal size";
    if (Elem >= 0 && Elem < MaxPower)
        BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    if (Elem < 0 || Elem >= MaxPower)
        throw "Non equal size";
    if (Elem >= 0 && Elem < MaxPower)
        BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s) // присваивание
{
    if (this != &s) 
    {
        MaxPower = s.MaxPower;
        BitField = s.BitField;
    }
    return *this;
}

int TSet::operator==(const TSet &s) const // сравнение
{
    return BitField == s.BitField;


    //return (MaxPower == s.MaxPower) && (BitField == s.BitField);
}

int TSet::operator!=(const TSet &s) const // сравнение
{
    return BitField != s.BitField;


    //return !(*this == s);
}

TSet TSet::operator+(const TSet& s) // объединение
{
    TSet res = TSet(std::max(MaxPower, s.MaxPower));
    res.BitField = BitField | s.BitField;
    return res;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    TSet result(*this);  
    result.InsElem(Elem);  
    return result;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    TSet result(*this); 
    result.DelElem(Elem); 
    return result;
}

TSet TSet::operator*(const TSet &s) // пересечение
{
    TSet res(MaxPower);
    res.BitField = BitField & s.BitField; // Пересечение битовых полей
    return res;
}

TSet TSet::operator~(void) // дополнение
{
    TSet result(MaxPower);
    result.BitField = ~BitField;  // Инвертируем все биты
    return result;
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s) // ввод
{
    int elem;
    while (istr >> elem) 
    {
        if (elem >= 0 && elem < s.MaxPower) 
        {
            s.InsElem(elem);
        }
    }
    return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{
    ostr << "{";
    for (int i = 0; i < s.MaxPower; i++) 
    {
        if (s.IsMember(i)) 
        {
            if (i > 0) ostr << ", ";
            ostr << i;
        }
    }
    ostr << "}";
    return ostr;
}
