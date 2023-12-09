#include <xbyak/xbyak.h>
#include "RE/Shims.h"
#include "LoaderHooks.h"

namespace LoaderHooks
{
	class ReadFormHeaderHook : public Xbyak::CodeGenerator
	{
	public:
		static void Thunk(RE::TESFile *a_masterFile, RE::TESFile *a_currentFile)
		{
			// Prior to 1.6.1126, AdjustFormIDFileIndex would always be called and the masked IsDefaultForm
			// check would follow.
			if (!RE::TESForm__IsDefaultForm(a_currentFile->currentform.formID))
				RE::TESFile__AdjustFormIDFileIndex(a_masterFile, &a_currentFile->currentform.formID);
		}

		static std::uintptr_t GetTarget()
		{
			// 14018A9A4 1.6.640
			return RELOCATION_ID(13935, 14033).address() + REL::VariantOffset(0x141, 0x144, 0).offset();
		}

		static std::uintptr_t GetReturn()
		{
			// 14018AA0E 1.6.640
			return RELOCATION_ID(13935, 14033).address() + REL::VariantOffset(0x169, 0x1AE, 0).offset();
		}

		ReadFormHeaderHook()
		{
			if (REL::Module::IsAE())
			{
				mov(rdx, rbx); // Current file
				mov(rcx, rcx); // Parent file (if applicable)
				mov(rax, reinterpret_cast<std::uintptr_t>(&Thunk));
				call(rax);
			}
			else if (REL::Module::IsSE())
			{
				mov(rdx, rdi);
				mov(rcx, rcx);
				mov(rax, reinterpret_cast<std::uintptr_t>(&Thunk));
				call(rax);
			}

			mov(rax, GetReturn());
			jmp(rax);
		}
	};

	class ReadFormVersionHook
	{
	private:
		constexpr static float HedrVersion16659 = 1.70f;	// <= 1.6.659
		constexpr static float HedrVersion161126 = 1.71f;	// >= 1.6.1126

	public:
		static bool Thunk(RE::TESFile *a_this, void *a_data, std::uint32_t a_maxSize)
		{
			if (!a_this->ReadData(a_data, a_maxSize))
				return false;

			// Patch the HEDR record data
			auto& version = *reinterpret_cast<float *>(a_data);

			if (version > HedrVersion16659 && version <= HedrVersion161126)
			{
				version = HedrVersion16659;
				SKSE::log::info("Emulated old header version for {}.", a_this->fileName);
			}

			return true;
		}

		static std::uintptr_t GetTarget()
		{
			// 140186DF6 1.6.640
			return RELOCATION_ID(13860, 13936).address() + REL::VariantOffset(0x44E, 0x5D6, 0).offset();
		}
	};

	void Install()
	{
		auto& trampoline = SKSE::GetTrampoline();

		ReadFormHeaderHook hook;
		hook.ready();
		trampoline.write_branch<5>(hook.GetTarget(), trampoline.allocate(hook));

		trampoline.write_call<5>(ReadFormVersionHook::GetTarget(), &ReadFormVersionHook::Thunk);
	}
}
