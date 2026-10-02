// Copyright Ian Good

#pragma once

#include "IGNetIco.h"
#include "IGPrintf/IGPrintf.h"
#include "Logging/LogMacros.h"

DECLARE_LOG_CATEGORY_EXTERN(LogIGShowme, Log, All)

#define IGSHOWME_LOG_CONSTRUCTOR UE_LOG(LogIGShowme, Log, TEXT("%s: %s 🎉 %s {Outer=%s}"), IG_NET_ICO(this), IGSHOWME_SUBJECTICO, *GetName(), *GetNameSafe(GetOuter()))

#define IGSHOWME_LOG_DESTRUCTOR UE_LOG(LogIGShowme, Log, TEXT("%s: %s 💀 %s / %s"), *NetIcoDuringDestruction, IGSHOWME_SUBJECTICO, *OriginalName.ToString(), *NameDuringDestruction.ToString())

#define IGSHOWME_LOG_FINISHDESTROY UE_LOG(LogIGShowme, Log, TEXT("%s: %s %s %hs()"), *NetIcoDuringDestruction, IGSHOWME_SUBJECTICO, *OriginalName.ToString(), __FUNCTION__)

/**
 * Logs a message with the net mode, object name, and function name.
 * Requires a `TCHAR*` `IGSHOWME_SUBJECTICO` visible in scope.
 * Requires `this` is a `UObject`.
 *
 * @example
 *	IGSHOWME_LOG_FUNCTION_CALL;
 *	LogIGShowme: 👶: 🎭 MyActor0 AIGShowmeActor::BeginPlay()
 */
#define IGSHOWME_LOG_FUNCTION_CALL UE_LOG(LogIGShowme, Log, TEXT("%s: %s %s %hs()"), IG_NET_ICO(this), IGSHOWME_SUBJECTICO, *GetName(), __FUNCTION__)

/**
 * Slimmed down version of `IGSHOWME_LOG_FUNCTION_CALL` for use when no UObject is available.
 */
#define IGSHOWME_LOG_FUNCTION_CALL_SLIM UE_LOG(LogIGShowme, Log, TEXT("%s %hs()"), IGSHOWME_SUBJECTICO, __FUNCTION__)

/**
 * Logs a message with the net mode, object name, function name, and formatted arguments.
 * Requires a `TCHAR*` `IGSHOWME_SUBJECTICO` visible in scope.
 * Requires `this` is a `UObject`.
 *
 * @example
 *	IGSHOWME_LOG_FUNCTION_CALL_ARGS("reason=%s", IG_FMT(EndPlayReason));
 *	LogIGShowme: 👶: 🎭 MyActor0 AIGShowmeActor::EndPlay(reason=EndPlayInEditor)
 */
#define IGSHOWME_LOG_FUNCTION_CALL_ARGS(Fmt, ...) UE_LOG(LogIGShowme, Log, TEXT("%s: %s %s %hs(" Fmt ")"), IG_NET_ICO(this), IGSHOWME_SUBJECTICO, *GetName(), __FUNCTION__, __VA_ARGS__)

#define IGSHOWME_LOG_MEMBER(Fmt, Member) UE_LOG(LogIGShowme, Log, TEXT("%s: %s %s { " #Member "=" Fmt " }"), IG_NET_ICO(this), IGSHOWME_SUBJECTICO, *GetName(), Member)
