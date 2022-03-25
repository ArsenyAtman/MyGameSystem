// Fill out your copyright notice in the Description page of Project Settings.


#include "StatsSystemTypes.h"

FStatValues FStatValues::operator + (FStatValues StatValues)
{
	FStatValues NewStatValues = *this;

	NewStatValues.Min += StatValues.Min;
	NewStatValues.Max += StatValues.Max;
	NewStatValues.Value += StatValues.Value;

	return NewStatValues;
}

FStatValues FStatValues::operator - (FStatValues StatValues)
{
	FStatValues NewStatValues = *this;

	NewStatValues.Min -= StatValues.Min;
	NewStatValues.Max -= StatValues.Max;
	NewStatValues.Value -= StatValues.Value;

	return NewStatValues;
}

FStatValues FStatValues::operator * (FStatValues StatValues)
{
	FStatValues NewStatValues = *this;

	NewStatValues.Min *= StatValues.Min;
	NewStatValues.Max *= StatValues.Max;
	NewStatValues.Value *= StatValues.Value;

	return NewStatValues;
}

FStatValues FStatValues::operator / (FStatValues StatValues)
{
	FStatValues NewStatValues = *this;

	NewStatValues.Min /= StatValues.Min;
	NewStatValues.Max /= StatValues.Max;
	NewStatValues.Value /= StatValues.Value;

	return NewStatValues;
}

FStatValues FStatValues::operator * (float Scale)
{
	FStatValues NewStatValues = *this;

	NewStatValues.Min *= Scale;
	NewStatValues.Max *= Scale;
	NewStatValues.Value *= Scale;

	return NewStatValues;
}

FStatValues operator * (float Scale, FStatValues StatValues)
{
	StatValues.Min *= Scale;
	StatValues.Max *= Scale;
	StatValues.Value *= Scale;

	return StatValues;
}

FStatValues& FStatValues::operator += (FStatValues StatValues)
{
	*this = *this + StatValues;

	return *this;
}

FStatValues& FStatValues::operator -= (FStatValues StatValues)
{
	*this = *this - StatValues;

	return *this;
}

FStatValues& FStatValues::operator *= (FStatValues StatValues)
{
	*this = *this * StatValues;

	return *this;
}

FStatValues& FStatValues::operator /= (FStatValues StatValues)
{
	*this = *this / StatValues;

	return *this;
}

FStatValues& FStatValues::operator *= (float Scale)
{
	*this = *this * Scale;

	return *this;
}