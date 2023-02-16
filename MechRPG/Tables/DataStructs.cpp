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

EArmourSlot UDataStructs::GetArmourSlot(FString typeName)
{
	if (typeName.Equals("Chest"))
	{
		return EArmourSlot::Chest;
	}
	else if (typeName.Equals("Head"))
	{
		return EArmourSlot::Head;
	}
	else if (typeName.Equals("LeftArm"))
	{
		return EArmourSlot::Left_Arm;
	}
	else if (typeName.Equals("RightArm"))
	{
		return EArmourSlot::Right_Arm;
	}
	else if (typeName.Equals("LeftLeg"))
	{
		return EArmourSlot::Left_Leg;
	}
	else if (typeName.Equals("RIghtLeg"))
	{
		return EArmourSlot::RIght_Leg;
	}
	return EArmourSlot::Head;
}

EDamageType UDataStructs::GetDamageType(FString typeName)
{
	if (typeName.Equals("Energy"))
	{
		return EDamageType::Energy;
	}
	else if (typeName.Equals("Fire"))
	{
		return EDamageType::Fire;
	}
	else if (typeName.Equals("Kinetic"))
	{
		return EDamageType::Kinetic;
	}
	return EDamageType::Kinetic;
}

ECharacterType UDataStructs::GetCharacterType(FString typeName)
{
	if (typeName.Equals("Player"))
	{
		return ECharacterType::Player;
	}
	else if (typeName.Equals("Ally"))
	{
		return ECharacterType::Ally;
	}
	else if (typeName.Equals("Enemy"))
	{
		return ECharacterType::Enemy;
	}
	else if (typeName.Equals("Neutral"))
	{
		return ECharacterType::Neutral;
	}
	return ECharacterType::Neutral;
}

bool UDataStructs::GetBoolean(FString value)
{
	return value.Equals("true") ? true : false;
}
