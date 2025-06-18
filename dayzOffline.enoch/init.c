void main()
{
	//INIT ECONOMY--------------------------------------
	Hive ce = CreateHive();
	if ( ce )
		ce.InitOffline();

	//DATE RESET AFTER ECONOMY INIT-------------------------
	int year, month, day, hour, minute;
	int reset_month = 7, reset_day = 17;
	GetGame().GetWorld().GetDate(year, month, day, hour, minute);

	if ((month == reset_month) && (day < reset_day))
	{
		GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
	}
	else
	{
		if ((month == reset_month + 1) && (day > reset_day))
		{
			GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
		}
		else
		{
			if ((month < reset_month) || (month > reset_month + 1))
			{
				GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
			}
		}
	}
}

// Add your BuilderItems or DayZ Editor Code Here for building and item placement

class CustomMission: MissionServer
{
	// Character Creation
	int MELEE_CHANCE 			        		= 50;		// Chance of getting a melee weapon. Out of 100. default is 100%. To Disable set to 0
	
	
	// Seasonal Settings
	bool CHRISTMAS							= false;	// Enables christmas gear on spawn. To Disable set to false

	// Spawn Clothing Config	
	autoptr array<string> GEAR_TOPS  	   	= {"Shirt_BlueCheck", "Shirt_BlueCheckBright", "Shirt_GreenCheck", "Shirt_RedCheck", "Shirt_WhiteCheck", "Shirt_PlaneBlack", "TShirt_Beige", "TShirt_Black", "TShirt_Blue", "Tshirt_Green", "TShirt_Red", "Tshirt_White", "Tshirt_Grey", "TacticalShirt_Grey", "TacticalShirt_Black", "TacticalShirt_Olive", "TacticalShirt_Tan"};
	autoptr array<string> GEAR_PANTS 		= {"CargoPants_Black", "CargoPants_Blue", "CargoPants_Green", "CargoPants_Grey", "Jeans_Black", "Jeans_Blue", "Jeans_Brown", "Jeans_Grey"};
	autoptr array<string> GEAR_GLOVES		= {"WorkingGloves_Black", "WorkingGloves_Brown", "WorkingGloves_Beige" , "WorkingGloves_Yellow", "TacticalGloves_Black", "TacticalGloves_Beige"};
	autoptr array<string> GEAR_SHOES 		= {"AthleticShoes_Black", "AthleticShoes_Green", "AthleticShoes_Blue", "AthleticShoes_Brown", "AthleticShoes_Grey"};
	autoptr array<string> GEAR_MELEE 		= {"StoneKnife", "KitchenKnife", "SteakKnife"};
	autoptr array<string> GEAR_BACKPACK   	= {"TaloonBag_Blue", "TaloonBag_Green", "TaloonBag_Orange", "MountainBag_Red", "MountainBag_Blue", "MountainBag_Orrange", "MountainBag_Green"};
	autoptr array<string> CHEMLIGHT			= {"Chemlight_White", "Chemlight_Yellow", "Chemlight_Green", "Chemlight_Red"};
	autoptr array<string> FOOD    	    	= {"PeachesCan", "SpaghettiCan", "TacticalBaconCan", "BakedBeansCan"};
	autoptr array<string> DRINK    			= {"SodaCan_Cola", "SodaCan_Pipsi", "SodaCan_Spite", "SodaCan_Kvass"};
	autoptr array<string> MEDICAL    		= {"Bandage", "BandageDressing", "Rag"};
	autoptr array<string> Map    			= {"ChernarusMap"};
	autoptr array<string> Compas   			= {"OrienteeringCompass", "Compass"};
	autoptr array<string> RRadio  			= {"PersonalRadio"};
	autoptr array<string> Pen  				= {"Pen_Black", "Pen_Red", "Pen_Green", "Pen_Blue"};

	// Global Variables
	EntityAI itemTop;
	EntityAI itemBag;
	EntityAI itemEnt, Char_Bag, Char_Gloves, Char_Top, Char_Pants, Char_Shoes, Char_Chemlight, Char_Melee, attachment;
	ItemBase itemBs;

	// Convert variables to string
	string	RandomMelee;
	string  RandomChemlight


	// Use for randomizing items chances.
	int randNum = Math.RandomInt( 0, 100 );
	
	// Set how many bandages or rags player starts with.
	const int MEDICAL_AMOUNT = 4;	

	// Set Character Health
	void SetRandomHealth(EntityAI itemEnt)
	{
		if ( itemEnt )
		{
			float rndHlt = Math.RandomFloat( 0.75, 100 );
			itemEnt.SetHealth( "", "", rndHlt );
		}
	}

	// Set Character into game
	override PlayerBase CreateCharacter(PlayerIdentity identity, vector pos, ParamsReadContext ctx, string characterName)
	{
		Entity playerEnt;
		playerEnt = GetGame().CreatePlayer( identity, characterName, pos, 0, "NONE" );
		Class.CastTo( m_player, playerEnt );
		GetGame().SelectPlayer( identity, m_player );
		return m_player;
	}

	EntityAI A6_Glock19Class(PlayerBase player)
	{
		EntityAI pistol = player.GetHumanInventory().CreateInHands("A6_Glock19");

		EntityAI plight = pistol.GetInventory().CreateAttachment("A6_Foxtrot_PistolLight");

		EntityAI psight = pistol.GetInventory().CreateAttachment("A6_RMROptic_Black");

		plight.GetInventory().CreateAttachment("Battery9V"); 
		psight.GetInventory().CreateAttachment("Battery9V");

		pistol.GetInventory().CreateAttachment("A6_Mag_Glock19_15Rnd");

		// Add 9x19 ammo to the player's inventory
		player.GetInventory().CreateInInventory("Ammo_9x19");
		if (Math.RandomInt(0, 2) == 0)  // Generates a random number, 0 or 1
		{
			pistol.GetInventory().CreateAttachment("A6_OspreySuppressor");
		}

		return pistol;
	}

	EntityAI A6_M9Class(PlayerBase player)
	{
		EntityAI pistol = player.GetHumanInventory().CreateInHands("A6_M9");
		pistol.GetInventory().CreateAttachment("A6_Mag_M9_15Rnd");
		player.GetInventory().CreateInInventory("Ammo_9x19");
		
		return pistol;
	}

	EntityAI Colt1911Class(PlayerBase player)
	{
		EntityAI pistol = player.GetHumanInventory().CreateInHands("Colt1911");
		EntityAI plight = pistol.GetInventory().CreateAttachment("TLRLight");
		plight.GetInventory().CreateAttachment("Battery9V"); 
		pistol.GetInventory().CreateAttachment("Mag_1911_7Rnd");
		player.GetInventory().CreateInInventory("Ammo_45ACP");

		return pistol;
	}

	EntityAI Glock19Class(PlayerBase player)
	{
		EntityAI pistol = player.GetHumanInventory().CreateInHands("Glock19");
		EntityAI plight = pistol.GetInventory().CreateAttachment("TLRLight");
		plight.GetInventory().CreateAttachment("Battery9V"); 
		pistol.GetInventory().CreateAttachment("Mag_Glock_15Rnd");
		player.GetInventory().CreateInInventory("Ammo_9x19");

		return pistol;
	}

	EntityAI FNX45Class(PlayerBase player)
	{
		EntityAI pistol = player.GetHumanInventory().CreateInHands("FNX45");
		EntityAI plight = pistol.GetInventory().CreateAttachment("TLRLight");
		plight.GetInventory().CreateAttachment("Battery9V"); 
		pistol.GetInventory().CreateAttachment("Mag_FNX45_15Rnd");
		player.GetInventory().CreateInInventory("Ammo_45ACP");

		return pistol;
	}

	override void StartingEquipSetup(PlayerBase player, bool clothesChosen)
	{	
		EntityAI itemClothing;
		EntityAI itemTop;
		EntityAI itemBag;
		EntityAI itemEnt, Char_Bag, Char_Gloves, Char_Top, Char_Pants, Char_Shoes, Char_Chemlight, Char_Melee, attachment;
		ItemBase itemBs;

		// Start By Removing all default items regardess of name or state
		player.RemoveAllItems(); 
	
		// If Xmas is toggled on.  Use Christmas Clothing Set! Else use random custom hoodie, or other top
		if ( CHRISTMAS ) {
			player.GetInventory().CreateInInventory( "SantasHat" );
			player.GetInventory().CreateInInventory( "SantasBeard" );
		} else {
			Char_Top = player.GetInventory().CreateInInventory( GEAR_TOPS.GetRandomElement());
		}

		// Set Random Base Clothing Loadout
		Char_Pants 	  	= player.GetInventory().CreateInInventory( GEAR_PANTS.GetRandomElement());
		Char_Shoes 	  	= player.GetInventory().CreateInInventory( GEAR_SHOES.GetRandomElement());
		Char_Gloves   	= player.GetInventory().CreateInInventory( GEAR_GLOVES.GetRandomElement());
		Char_Bag  	    = player.GetInventory().CreateInInventory( GEAR_BACKPACK.GetRandomElement());

		// Find player shirt slot
		itemTop = player.FindAttachmentBySlotName("Body");

		// If Shirt or Jacket is attached
		if ( itemTop )
			{
			// Give 4 Random Medical Items
			itemEnt = player.GetInventory().CreateInInventory( MEDICAL.GetRandomElement());
			if ( Class.CastTo(itemBs, itemEnt ) )
			itemBs.SetQuantity(MEDICAL_AMOUNT);
			// Give Random Food and Drink Items
			itemEnt = player.GetInventory().CreateInInventory( FOOD.GetRandomElement());
			itemEnt = player.GetInventory().CreateInInventory( DRINK.GetRandomElement());
		}

		// Find player Back slot
		itemBag = player.FindAttachmentBySlotName("Back");
		// If Backpack is attached
		if ( itemBag )
			{
			//give random pen, radio, map, compass
			itemEnt = itemBag.GetInventory().CreateInInventory( Map.GetRandomElement());
			itemEnt = itemBag.GetInventory().CreateInInventory( Compas.GetRandomElement());
			itemEnt = itemBag.GetInventory().CreateInInventory( RRadio.GetRandomElement());
			itemEnt.GetInventory().CreateAttachment("Battery9V");
			itemEnt = itemBag.GetInventory().CreateInInventory( Pen.GetRandomElement());
		}

		// Set character Melee Weapon and add it to quickbar (Can reuse for other weapons)
		// Does the player get a Melee Weapon?
		if ( randNum <= MELEE_CHANCE ) {
			// Get melee weapon
			RandomMelee = GEAR_MELEE.GetRandomElement();
			// Create in inventory
		 	Char_Melee = player.GetInventory().CreateInInventory( RandomMelee );
			// Assign to quick bar
			player.SetQuickBarEntityShortcut( Char_Melee, 2, true );
			// Put item in hands (OPTIONAL)
			if ( GetGame().IsMultiplayer() ) {	
				player.ServerTakeEntityToHands( Char_Melee );
			} else {
				player.LocalTakeEntityToHands( Char_Melee );
			}
		}	
		
		EntityAI primary;

		switch (Math.RandomInt(0, 5)) {
			case 0: primary = Colt1911Class(player); break;
			case 1: primary = Glock19Class(player); break;
			case 2: primary = FNX45Class(player); break;
			case 3: primary = A6_M9Class(player); break;
			case 4: primary = A6_Glock19Class(player); break;
		}

		player.LocalTakeEntityToHands(primary);
		player.SetQuickBarEntityShortcut(primary, 0, true);
	}
};

Mission CreateCustomMission(string path)
{
	return new CustomMission();
}	