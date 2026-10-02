// Copyright Ian Good

#pragma once

class UObject;

namespace IG
{
/**
 * Gets a simplified string representation of the world's net mode.
 *
 * This is intended for logging purposes so it's easy to see who is executing the code (server? client? standalone?).
 *
 * @example
 *  UE_LOG(LogItems, Error, TEXT("%s: Item not found."), IG_NET_ICO(this));
 *  server might say     "LogItems: Error: 🎅: Item not found."
 *  client might say     "LogItems: Error: 👶: Item not found."
 *  standalone might say "LogItems: Error: 🎅👶: Item not found."
 */
const TCHAR* GetNetModeIcons(const UObject* WorldContextObject);

} // namespace IG

/** See `IG::GetNetModeIcons`. */
#define IG_NET_ICO(WCO) IG::GetNetModeIcons(WCO)
