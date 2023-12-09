#include <spdlog/sinks/basic_file_sink.h>
#include "Impl/ContainerHooks.h"
#include "Impl/PapyrusImpl.h"
#include "Impl/LoaderHooks.h"

using namespace SKSE;

void InitializeLog()
{
	auto path = log::log_directory();

	if (!path)
		stl::report_and_fail("Unable to find SKSE logs directory.");

	*path /= PluginDeclaration::GetSingleton()->GetName();
	*path += L".log";

	auto logger = spdlog::basic_logger_mt("file_logger", path.value().string(), true);
	logger->set_level(spdlog::level::level_enum::info);
	logger->set_pattern("[%H:%M:%S] [%l] %v"); // [HH:MM:SS] [Level] Message
	logger->flush_on(logger->level());
	spdlog::set_default_logger(std::move(logger));
}

SKSEPluginLoad(const LoadInterface *a_interface)
{
	InitializeLog();

	log::info(
		"Backported Extended ESL Support version {}.{} by Nukem. Mod URL: "
		"https://www.nexusmods.com/skyrimspecialedition/mods/106441",
		PluginDeclaration::GetSingleton()->GetVersion().major(),
		PluginDeclaration::GetSingleton()->GetVersion().minor());

	const auto currentVersion = REL::Module::get().version();
	const auto newEslSupportVersion = REL::Version(1, 6, 1126);

	if (currentVersion >= newEslSupportVersion)
	{
		log::error("Game version {} already has native support. Skipping hooks.", currentVersion.string(), newEslSupportVersion.string());
		return true;
	}

	Init(a_interface);
	AllocTrampoline(128);

	auto papyrusInterface = GetPapyrusInterface();
	papyrusInterface->Register(PapyrusImpl::RegisterFunctions);

	LoaderHooks::Install();
	ContainerHooks::Install();
	return true;
}
