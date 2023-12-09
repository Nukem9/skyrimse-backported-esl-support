#include <xbyak/xbyak.h>
#include "RE/Shims.h"
#include "ContainerHooks.h"

namespace ContainerHooks
{
	class TESContainerCtorHook : public Xbyak::CodeGenerator
	{
	public:
		static std::uintptr_t GetTarget()
		{
			// 14019B510 1.6.640
			return RELOCATION_ID(14370, 14510).address();
		}

		TESContainerCtorHook()
		{
			// This is a leaf function. All other registers are preserved.
			mov(rax, RE::VTABLE_TESContainer[0].address());
			mov(ptr[rcx + 0x0], rax);												// VTable

			xor_(rax, rax);
			mov(ptr[rcx + offsetof(RE::TESContainer, containerObjects)], rax);		// ContainerObjects
			mov(dword[rcx + offsetof(RE::TESContainer, numContainerObjects)], eax);	// NumContainerObjects
			mov(byte[rcx + offsetof(RE::TESContainer, pad14)], al);					// BlockStolenItems

			mov(rax, rcx);
			ret();
		}
	};

	class ContainerMenuListEnumerationHook
	{
	public:
		static bool Thunk(RE::UnknownInventoryMenuData *a_this, RE::InventoryEntryData *a_entryData)
		{
			if (!a_this->Populate(a_entryData))
				return false;

			// 1.6.1126 implements this by appending a new struct member at UnknownInventoryMenuData+0x17 which gets
			// set much further up in the call chain. The code below is an equivalent workaround.
			const auto container = a_this->refr ? static_cast<RE::TESContainer1126 *>(a_this->refr->GetContainer()) : nullptr;
			const auto itemData = a_entryData->extraLists;

			if (itemData && container && container->GetBlockStolenItems())
			{
				const auto player = RE::PlayerCharacter::GetSingleton();

				//
				// Rebuild the whole list because it's far easier than trying to remove elements. BSSimpleList
				// in CommonLib is currently a shitshow.
				//
				// 1. CommonLib doesn't declare a non-const erase_after() function. There's two options: patch it
				//    or use a hacked reinterpret_cast to convert a non-const iterator to a const one.
				//
				// 2. erase_after() doesn't call free when deleting single elements. It may not even unlink them
				//    from the list. Definitely a memory leak.
				//
				// 3. Iterators don't seem to behave correctly at all. I'm not sure if it's buggy or behavior is
				//    different than the C++ STL.
				//
				const auto newItemDataList = new RE::BSSimpleList<RE::ExtraDataList *>();
				int blockedItemCount = 0;

				for (auto itr = itemData->begin(); itr != itemData->end(); ++itr)
				{
					const auto extraOwnership = (*itr)->GetByType<RE::ExtraOwnership>();

					if (a_entryData->IsOwnedBy(player, extraOwnership ? extraOwnership->owner : nullptr, true))
					{
						newItemDataList->push_front(*itr);
					}
					else
					{
						const auto extraCount = (*itr)->GetByType<RE::ExtraCount>();
						blockedItemCount += extraCount ? extraCount->count : 1;
					}
				}

				a_entryData->extraLists = newItemDataList;
				delete itemData;

				if (a_entryData->countDelta > blockedItemCount)
				{
					a_entryData->countDelta -= blockedItemCount;
					return true;
				}

				return false;
			}

			return true;
		}

		static auto GetTargets()
		{
			// 1408A0696 1.6.640
			// 1408A07C8 1.6.640
			return std::array { 
				RELOCATION_ID(50227, 51156).address() + REL::VariantOffset(0xB6, 0xA6, 0).offset(),
				RELOCATION_ID(50228, 51157).address() + REL::VariantOffset(0xB6, 0xA8, 0).offset(),
			};
		}
	};

	void Install()
	{
		auto& trampoline = SKSE::GetTrampoline();

		TESContainerCtorHook hook;
		hook.ready();
		trampoline.write_branch<5>(hook.GetTarget(), trampoline.allocate(hook));

		for (auto target : ContainerMenuListEnumerationHook::GetTargets())
			trampoline.write_call<5>(target, &ContainerMenuListEnumerationHook::Thunk);
	}
}
