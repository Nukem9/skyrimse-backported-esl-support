#pragma once

namespace RE
{
	class TESContainer1126 : public RE::TESContainer
	{
	public:
		// New constructor is handled in ContainerHooks.cpps
		void SetBlockStolenItems(bool a_blockStolenItems)
		{
			*reinterpret_cast<bool *>(&pad14) = a_blockStolenItems;
		}

		bool GetBlockStolenItems() const
		{
			return *reinterpret_cast<const bool *>(&pad14);
		}
	};
	static_assert(sizeof(TESContainer1126) == sizeof(TESContainer));

	class UnknownInventoryMenuData
	{
	public:
		// members
		RE::TESObjectREFR *refr;	// 00

		bool Populate(RE::InventoryEntryData *a_entryData)
		{
			static REL::Relocation<decltype(&UnknownInventoryMenuData::Populate)> func { RELOCATION_ID(50239, 51171) };
			return func(this, a_entryData);
		}
	};

	static void TESFile__AdjustFormIDFileIndex(TESFile *a_this, FormID *a_formID)
	{
		static REL::Relocation<decltype(&TESFile__AdjustFormIDFileIndex)> func { RELOCATION_ID(13915, 14003) };
		return func(a_this, a_formID);
	}

	static bool TESForm__IsDefaultForm(FormID a_formID)
	{
		static REL::Relocation<decltype(&TESForm__IsDefaultForm)> func { RELOCATION_ID(14494, 14652) };
		return func(a_formID);
	}

	static void PlayerCharacter__CheckForQuestTargetUpdate(RE::PlayerCharacter *a_this, RE::TESObjectREFR *a_movingRef)
	{
		static REL::Relocation<decltype(&PlayerCharacter__CheckForQuestTargetUpdate)> func { RELOCATION_ID(39436, 40512) };
		return func(a_this, a_movingRef);
	}

	static std::int32_t InventoryChanges__CountUnknown(RE::InventoryChanges *a_this, bool a_unknown1, bool a_unknown2)
	{
		static REL::Relocation<decltype(&InventoryChanges__CountUnknown)> func { RELOCATION_ID(15865, 16105) };
		return func(a_this, a_unknown1, a_unknown2);
	}

	static void InventoryChanges__RemoveItemsConditionally(
		RE::InventoryChanges *a_this,
		RE::TESObjectREFR *a_removeFrom,
		RE::TESObjectREFR *a_transferTo,
		RE::ExtraOwnership *a_ownershipData,
		bool a_removeStolenOnly,
		bool a_showHudMessage)
	{
		static REL::Relocation<decltype(&InventoryChanges__RemoveItemsConditionally)> func { RELOCATION_ID(15880, 16120) };
		return func(a_this, a_removeFrom, a_transferTo, a_ownershipData, a_removeStolenOnly, a_showHudMessage);
	}
}
