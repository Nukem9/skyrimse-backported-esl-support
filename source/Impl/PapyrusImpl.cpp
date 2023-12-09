#include "RE/Shims.h"
#include "PapyrusImpl.h"

namespace PapyrusImpl
{
	void SetContainerAllowStolenItems(
		RE::BSScript::IVirtualMachine *a_vm,
		RE::VMStackID a_stackID,
		RE::TESObjectREFR *a_refr,
		bool a_allowStolenItems)
	{
		if (auto container = a_refr->GetContainer())
			static_cast<RE::TESContainer1126 *>(container)->SetBlockStolenItems(!a_allowStolenItems);
	}

	std::uint32_t GetAllItemsCount(RE::BSScript::IVirtualMachine *a_vm, RE::VMStackID a_stackID, RE::TESObjectREFR *a_refr)
	{
		if (!a_refr->GetContainer())
		{
			a_vm->TraceForm(a_refr, "is not a container", a_stackID, RE::BSScript::ErrorLogger::Severity::kError);
			return 0;
		}

		if (auto inventoryChanges = a_refr->GetInventoryChanges(true))
			return InventoryChanges__CountUnknown(inventoryChanges, true, false);

		return 0;
	}

	bool IsContainerEmpty(RE::BSScript::IVirtualMachine *a_vm, RE::VMStackID a_stackID, RE::TESObjectREFR *a_refr)
	{
		return GetAllItemsCount(a_vm, a_stackID, a_refr) == 0;
	}

	void RemoveAllStolenItems(
		RE::BSScript::IVirtualMachine *a_vm,
		RE::VMStackID a_stackID,
		RE::TESObjectREFR *a_refr,
		RE::TESObjectREFR *a_transferTo)
	{
		if (auto inventoryChanges = a_refr->GetInventoryChanges(true))
		{
			InventoryChanges__RemoveItemsConditionally(inventoryChanges, a_refr, a_transferTo, nullptr, true, false);
			PlayerCharacter__CheckForQuestTargetUpdate(RE::PlayerCharacter::GetSingleton(), a_refr);
		}
	}

	bool RegisterFunctions(RE::BSScript::IVirtualMachine *a_vm)
	{
		a_vm->RegisterFunction("SetContainerAllowStolenItems", "ObjectReference", SetContainerAllowStolenItems);
		a_vm->RegisterFunction("GetAllItemsCount", "ObjectReference", GetAllItemsCount);
		a_vm->RegisterFunction("IsContainerEmpty", "ObjectReference", IsContainerEmpty);
		a_vm->RegisterFunction("RemoveAllStolenItems", "ObjectReference", RemoveAllStolenItems);

		return true;
	}
}
