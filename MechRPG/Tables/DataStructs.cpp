#include "DataStructs.h"

EWeaponType UDataStructs::GetWeaponType(FString typeName)
{
	if(typeName.Equals("Melee"))
	{
		return EWeaponType::Melee;
	}
	else if(typeName.Equals("Laser"))
	{
		return EWeaponType::Laser;		
	}
	else if (typeName.Equals("Projectile"))
	{
		return EWeaponType::Projectile;
	}
	return EWeaponType::Melee;
}

bool UDataStructs::GetBoolean(FString value)
{
	return value.Equals("TRUE") ? true : false;
}