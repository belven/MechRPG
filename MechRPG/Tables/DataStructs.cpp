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

EItemType UDataStructs::GetItemType(FString typeName)
{
	if (typeName.Equals("Weapon"))
	{
		return EItemType::Weapon;
	}
	else if (typeName.Equals("Consumable"))
	{
		return EItemType::Consumable;
	}
	else if (typeName.Equals("Armour"))
	{
		return EItemType::Armour;
	}
	return EItemType::Consumable;
}

bool UDataStructs::GetBoolean(FString value)
{
	return value.Equals("TRUE") ? true : false;
}