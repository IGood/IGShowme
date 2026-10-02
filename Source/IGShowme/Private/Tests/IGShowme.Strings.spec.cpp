// Copyright Ian Good

#if WITH_DEV_AUTOMATION_TESTS

#include "Math/RandomStream.h"
#include "Misc/AutomationTest.h"
#include "Tests/Benchmark.h"
#include "Tests/IGShowmeSpecimen.h"

static bool bGDoTheDebugLoggingPlease = false;

BEGIN_DEFINE_SPEC(FIGShowmeStringsSpec, "IGShowme.Strings", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter)

END_DEFINE_SPEC(FIGShowmeStringsSpec)

void FIGShowmeStringsSpec::Define()
{
	// TODO: add more things to grok about strings
}

/**
 * Compares various ways to create a string from a literal.
 */
IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FIGShowmeStringsBenchmark_CreateFromLiteral,
	"IGShowme.Strings.Benchmark.FromLiteral",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::PerfFilter)
bool FIGShowmeStringsBenchmark_CreateFromLiteral::RunTest(const FString& Parameters)
{
	constexpr int32 NumOperations = 100000;

	const auto CtorCString = []() {
		for (int32 i = 0; i < NumOperations; ++i)
		{
			const FString Result(TEXT("This string isn't super long but it's long enough for testing purposes that we might be able to see the difference between knowing the length ahead of time & needing to seek for the null terminator."));
			UE_CLOG(bGDoTheDebugLoggingPlease, LogTemp, Verbose, TEXT("%s"), *Result);
		}
	};

	const auto OpAssignCString = []() {
		for (int32 i = 0; i < NumOperations; ++i)
		{
			const FString Result = TEXT("This string isn't super long but it's long enough for testing purposes that we might be able to see the difference between knowing the length ahead of time & needing to seek for the null terminator.");
			UE_CLOG(bGDoTheDebugLoggingPlease, LogTemp, Verbose, TEXT("%s"), *Result);
		}
	};

	const auto CtorStringView = []() {
		for (int32 i = 0; i < NumOperations; ++i)
		{
			const FString Result(TEXTVIEW("This string isn't super long but it's long enough for testing purposes that we might be able to see the difference between knowing the length ahead of time & needing to seek for the null terminator."));
			UE_CLOG(bGDoTheDebugLoggingPlease, LogTemp, Verbose, TEXT("%s"), *Result);
		}
	};

	const auto CtorStringViewWSlack = []() {
		for (int32 i = 0; i < NumOperations; ++i)
		{
			const FString Result(TEXTVIEW("This string isn't super long but it's long enough for testing purposes that we might be able to see the difference between knowing the length ahead of time & needing to seek for the null terminator."), 0);
			UE_CLOG(bGDoTheDebugLoggingPlease, LogTemp, Verbose, TEXT("%s"), *Result);
		}
	};

	const auto AppendStringView = []() {
		for (int32 i = 0; i < NumOperations; ++i)
		{
			FString Result;
			Result.Append(TEXTVIEW("This string isn't super long but it's long enough for testing purposes that we might be able to see the difference between knowing the length ahead of time & needing to seek for the null terminator."));
			UE_CLOG(bGDoTheDebugLoggingPlease, LogTemp, Verbose, TEXT("%s"), *Result);
		}
	};

	UE_BENCHMARK(5, CtorCString);
	UE_BENCHMARK(5, OpAssignCString);
	UE_BENCHMARK(5, CtorStringView);
	UE_BENCHMARK(5, CtorStringViewWSlack);
	UE_BENCHMARK(5, AppendStringView);

	return true;
}

/**
 * Compares various ways to build a string with multiple pieces of data.
 */
IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FIGShowmeStringsBenchmark_Building,
	"IGShowme.Strings.Benchmark.Building",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::PerfFilter)
bool FIGShowmeStringsBenchmark_Building::RunTest(const FString& Parameters)
{
	constexpr int32 NumOperations = 100000;

	const auto Formatting = []() {
		const FName SomeName("asdf");
		for (int32 i = 0; i < NumOperations; ++i)
		{
			const FString Result = FString::Printf(TEXT("This is a test: '%s'%d"), *SomeName.ToString(), i);
			UE_CLOG(bGDoTheDebugLoggingPlease, LogTemp, Verbose, TEXT("%s"), *Result);
		}
	};

	const auto Appending = []() {
		const FName SomeName("asdf");
		for (int32 i = 0; i < NumOperations; ++i)
		{
			FString Result(TEXTVIEW("This is a test: '"));
			SomeName.AppendString(Result);
			Result.AppendChar(TEXT('\''));
			Result.AppendInt(i);
			UE_CLOG(bGDoTheDebugLoggingPlease, LogTemp, Verbose, TEXT("%s"), *Result);
		}
	};

	const auto StringBuildering = []() {
		const FName SomeName("asdf");
		for (int32 i = 0; i < NumOperations; ++i)
		{
			auto StringBuilderWBuffer = WriteToString<128>(TEXT("This is a test: '"), SomeName, TEXT('\''), i);
			const TCHAR* Result = *StringBuilderWBuffer;
			UE_CLOG(bGDoTheDebugLoggingPlease, LogTemp, Verbose, TEXT("%s"), Result);
		}
	};

	UE_BENCHMARK(5, Formatting);
	UE_BENCHMARK(5, Appending);
	UE_BENCHMARK(5, StringBuildering);

	return true;
}

/**
 * Compare various ways to concatenate 2 strings of known length.
 * This was written for Madlibs code where joining a string view with a literal is a very common operation.
 */
IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FIGShowmeStringsBenchmark_Concatenation,
	"IGShowme.Strings.Benchmark.Concatenation",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::PerfFilter)
bool FIGShowmeStringsBenchmark_Concatenation::RunTest(const FString& Parameters)
{
	constexpr int32 NumOperations = 100000;

	// Create a string using printf formatting.
	const auto PrintfFormat = []() {
		const FStringView VariableName = TEXTVIEW("SomeVariableName");
		for (int32 i = 0; i < NumOperations; ++i)
		{
			const FString Result = FString::Printf(TEXT("%.*s.SomeLiteral"), VariableName.Len(), VariableName.GetData());
			UE_CLOG(bGDoTheDebugLoggingPlease, LogTemp, Verbose, TEXT("%s"), *Result);
		}
	};

	// Create a string from a string view & append (length unknown);
	const auto StringAppend = []() {
		const auto Concat = [](const FStringView& SV, const TCHAR* NullTerminated) {
			return FString(SV) + NullTerminated;
		};
		const FStringView VariableName = TEXTVIEW("SomeVariableName");
		for (int32 i = 0; i < NumOperations; ++i)
		{
			const FString Result = Concat(VariableName, TEXT(".SomeLiteral"));
			UE_CLOG(bGDoTheDebugLoggingPlease, LogTemp, Verbose, TEXT("%s"), *Result);
		}
	};

	// Create a string from a string view (with slack) & append (length unknown).
	const auto StringAppendWithSlack = []() {
		const auto Concat = [](const FStringView& SV, auto&& Literal) {
			return FString(SV, /*ExtraSlack=*/UE_ARRAY_COUNT(Literal)) + static_cast<const TCHAR*>(Literal);
		};
		const FStringView VariableName = TEXTVIEW("SomeVariableName");
		for (int32 i = 0; i < NumOperations; ++i)
		{
			const FString Result = Concat(VariableName, TEXT(".SomeLiteral"));
			UE_CLOG(bGDoTheDebugLoggingPlease, LogTemp, Verbose, TEXT("%s"), *Result);
		}
	};

	// Create a string from a string view (with slack) & append (length known).
	const auto StringAppendCharsWithSlack = []() {
		const auto Concat = [](const FStringView& SV, auto&& Literal) {
			FString Result = FString(SV, /*ExtraSlack=*/UE_ARRAY_COUNT(Literal));
			Result.AppendChars(Literal, UE_ARRAY_COUNT(Literal) - 1);
			return Result;
		};
		const FStringView VariableName = TEXTVIEW("SomeVariableName");
		for (int32 i = 0; i < NumOperations; ++i)
		{
			const FString Result = Concat(VariableName, TEXT(".SomeLiteral"));
			UE_CLOG(bGDoTheDebugLoggingPlease, LogTemp, Verbose, TEXT("%s"), *Result);
		}
	};

	// Create a string from a string view (with slack) & append string view.
	const auto StringAppendSVWithSlack = []() {
		const auto Concat = [](const FStringView& SV, const FStringView& SV2) {
			FString Result = FString(SV, /*ExtraSlack=*/SV2.Len());
			return Result.Append(SV2);
		};
		const FStringView VariableName = TEXTVIEW("SomeVariableName");
		for (int32 i = 0; i < NumOperations; ++i)
		{
			const FString Result = Concat(VariableName, TEXTVIEW(".SomeLiteral"));
			UE_CLOG(bGDoTheDebugLoggingPlease, LogTemp, Verbose, TEXT("%s"), *Result);
		}
	};

	// averages from a test on 2025-11-12
	UE_BENCHMARK(5, PrintfFormat);               // 0.008153s
	UE_BENCHMARK(5, StringAppend);               // 0.006006s
	UE_BENCHMARK(5, StringAppendWithSlack);      // 0.003709s
	UE_BENCHMARK(5, StringAppendCharsWithSlack); // 0.003235s
	UE_BENCHMARK(5, StringAppendSVWithSlack);    // 0.005950s

	return true;
}

// These are our key/value pair types for our sorted arrays.
using FKeyIsEnum = TPair<EIGShowmeVeggies, FString>;
using FKeyIsString = TPair<FString, FString>;
using FKeyIsStringView = TPair<FStringView, FString>;
using FKeyIsName = TPair<FName, FString>;

// We need `op==` for our pairs so we can use `TArray::FindByKey`.
// clang-format off
static bool operator==(const FKeyIsEnum& Pair, EIGShowmeVeggies Key) { return Pair.Key == Key; }
static bool operator==(const FKeyIsString& Pair, const FString& Key) { return Pair.Key == Key; }
static bool operator==(const FKeyIsStringView& Pair, const FStringView& Key) { return Pair.Key == Key; }
static bool operator==(const FKeyIsName& Pair, const FName& Key) { return Pair.Key == Key; }
// clang-format on

/**
 * Compares lookups in TMap and sorted TArray with different key types:
 *  - Enum
 *  - FString
 *  - FStringView
 *  - FName
 *
 * Additionally, compares lookups where we convert from string view to other types before lookups.
 */
IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FIGShowmeStringsBenchmark_Lookups,
	"IGShowme.Strings.Benchmark.Lookups",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::PerfFilter)
bool FIGShowmeStringsBenchmark_Lookups::RunTest(const FString& Parameters)
{
	const auto GetNameStringByValue = [IGShowmeVeggies = StaticEnum<EIGShowmeVeggies>()](EIGShowmeVeggies V) {
		return IGShowmeVeggies->GetNameStringByValue(static_cast<int64>(V));
	};

	// These are our containers to test lookups against.
	// We'll have maps & sorted arrays.
	TMap<EIGShowmeVeggies, FString> KeyIsEnum;
	TMap<FString, FString> KeyIsString;
	TMap<FStringView, FString> KeyIsStringView;
	TMap<FName, FString> KeyIsName;

	TArray<FKeyIsEnum> ArrayKeyIsEnum;
	TArray<FKeyIsString> ArrayKeyIsString;
	TArray<FKeyIsStringView> ArrayKeyIsStringView;
	TArray<FKeyIsName> ArrayKeyIsName;

	for (const EIGShowmeVeggies V : TEnumRange<EIGShowmeVeggies>())
	{
		const FString VeggieString = GetNameStringByValue(V);

		KeyIsEnum.Add(V, VeggieString);
		KeyIsString.Add(VeggieString, VeggieString);
		////KeyIsStringView.Add(VeggieString, VeggieString) skip this - need to populate from `KeyIsString`
		KeyIsName.Add(*VeggieString, VeggieString);

		ArrayKeyIsEnum.Emplace(V, VeggieString);
		ArrayKeyIsString.Emplace(VeggieString, VeggieString);
		////ArrayKeyIsStringView.Emplace(VeggieString, VeggieString) skip this - need to populate from `KeyIsString`
		ArrayKeyIsName.Emplace(VeggieString, VeggieString);
	}

	// `KeyIsStringView` uses string views, which don't own their own memory.
	// We use the strings from `KeyIsString` to back them.
	for (const auto& [Key, Value] : KeyIsString)
	{
		KeyIsStringView.Add(Key, Value);
		ArrayKeyIsStringView.Emplace(Key, Value);
	}

	// Sort the arrays so we can do binary searches on them.
	Algo::SortBy(ArrayKeyIsEnum, &FKeyIsEnum::Key);
	Algo::SortBy(ArrayKeyIsString, &FKeyIsString::Key);
	Algo::SortBy(ArrayKeyIsStringView, &FKeyIsStringView::Key);
	Algo::SortBy(ArrayKeyIsName, &FKeyIsName::Key, FNameFastLess());

	constexpr int32 NumOperations = 100'000;

	// These are our lookup keys (simulates real world data).
	TArray<EIGShowmeVeggies> ElementIsEnum;
	ElementIsEnum.Reserve(NumOperations);
	TArray<FString> ElementIsString;
	ElementIsString.Reserve(NumOperations);
	TArray<FStringView> ElementIsStringView;
	ElementIsStringView.Reserve(NumOperations);
	TArray<FName> ElementIsName;
	ElementIsName.Reserve(NumOperations);

	const FRandomStream Rando(867'5309);
	for (int32 i = 0; i < NumOperations; ++i)
	{
		constexpr int32 ExclusiveUpperBound = static_cast<int32>(EIGShowmeVeggies::Count) + 1; // we use this to get random veggies, including `Count`
		const EIGShowmeVeggies V = static_cast<EIGShowmeVeggies>(Rando.RandHelper(ExclusiveUpperBound));
		ElementIsEnum.Emplace(V);
		const FString& VeggieString = ElementIsString.Emplace_GetRef(GetNameStringByValue(V));
		ElementIsStringView.Emplace(VeggieString); // Use the string in `ElementIsString` as our backing store.
		ElementIsName.Emplace(*VeggieString);
	}

	// Shared implementation for `Find` benchmarks.
	const auto LookupImpl = [](const TCHAR* What, const auto& Map, const auto& Elements) {
		int32 Hits = 0;
		for (const auto& V : Elements)
		{
			if (const FString* Found = Map.Find(V))
			{
				++Hits;
			}
		}

		UE_CLOG(bGDoTheDebugLoggingPlease, LogTemp, Verbose, TEXT("%s hit=%d"), What, Hits);
	};

	const auto LookupByEnum = [&]() {
		LookupImpl(TEXT("Enum"), KeyIsEnum, ElementIsEnum);
	};

	const auto LookupByString = [&]() {
		LookupImpl(TEXT("String"), KeyIsString, ElementIsString);
	};

	const auto LookupByStringView = [&]() {
		LookupImpl(TEXT("StringView"), KeyIsStringView, ElementIsStringView);
	};

	const auto LookupByName = [&]() {
		LookupImpl(TEXT("Name"), KeyIsName, ElementIsName);
	};

	const auto GetValueByNameString = [IGShowmeVeggies = StaticEnum<EIGShowmeVeggies>()](const FStringView& V) {
		return static_cast<EIGShowmeVeggies>(IGShowmeVeggies->GetValueByNameString(FString(V), EGetByNameFlags::CaseSensitive));
	};

	// Shared implementation for `Find` benchmarks that first convert the key to a string view.
	const auto LookupByConvertedImpl = [&](const TCHAR* What, const auto& Map, const auto& Elements, const auto& Converter) {
		int32 Hits = 0;
		for (const auto& V : Elements)
		{
			if (const FString* Found = Map.Find(Converter(V)))
			{
				++Hits;
			}
		}

		UE_CLOG(bGDoTheDebugLoggingPlease, LogTemp, Verbose, TEXT("StringView to %s hit=%d"), What, Hits);
	};

	const auto LookupByEnumFromStringView = [&]() {
		LookupByConvertedImpl(TEXT("Enum"), KeyIsEnum, ElementIsStringView, GetValueByNameString);
	};

	const auto LookupByStringFromStringView = [&]() {
		LookupByConvertedImpl(TEXT("String"), KeyIsString, ElementIsStringView, [](const FStringView& V) { return FString(V); });
	};

	const auto LookupByNameFromStringView = [&]() {
		LookupByConvertedImpl(TEXT("Name"), KeyIsName, ElementIsStringView, [](const FStringView& V) { return FName(V); });
	};

	// Shared implementation for `FindByKey` benchmarks.
	const auto FindByKeyImpl = [](const TCHAR* What, const auto& Array, const auto& Elements) {
		int32 Hits = 0;
		for (const auto& V : Elements)
		{
			if (const auto* Pair = Array.FindByKey(V))
			{
				const FString& Found = Pair->Value;
				++Hits;
			}
		}

		UE_CLOG(bGDoTheDebugLoggingPlease, LogTemp, Verbose, TEXT("%s hit=%d"), What, Hits);
	};

	const auto FindByEnum = [&]() {
		FindByKeyImpl(TEXT("Enum"), ArrayKeyIsEnum, ElementIsEnum);
	};

	const auto FindByString = [&]() {
		FindByKeyImpl(TEXT("String"), ArrayKeyIsString, ElementIsString);
	};

	const auto FindByStringView = [&]() {
		FindByKeyImpl(TEXT("StringView"), ArrayKeyIsStringView, ElementIsStringView);
	};

	const auto FindByName = [&]() {
		FindByKeyImpl(TEXT("Name"), ArrayKeyIsName, ElementIsName);
	};

	// Shared implementation for `BinarySearch` benchmarks.
	const auto BinarySearchImpl = [](const TCHAR* What, const auto& Array, const auto& Elements, auto SortPredicate) {
		int32 Hits = 0;
		for (const auto& V : Elements)
		{
			using FPair = std::remove_cvref_t<decltype(Array[0])>;
			const int32 Index = Algo::BinarySearchBy(Array, V, &FPair::Key, SortPredicate);
			if (Index != INDEX_NONE)
			{
				const FString& Found = Array[Index].Value;
				++Hits;
			}
		}

		UE_CLOG(bGDoTheDebugLoggingPlease, LogTemp, Verbose, TEXT("%s hit=%d"), What, Hits);
	};

	const auto SearchByEnum = [&]() {
		BinarySearchImpl(TEXT("Enum"), ArrayKeyIsEnum, ElementIsEnum, TLess<>());
	};

	const auto SearchByString = [&]() {
		BinarySearchImpl(TEXT("String"), ArrayKeyIsString, ElementIsString, TLess<>());
	};

	const auto SearchByStringView = [&]() {
		BinarySearchImpl(TEXT("StringView"), ArrayKeyIsStringView, ElementIsStringView, TLess<>());
	};

	const auto SearchByName = [&]() {
		BinarySearchImpl(TEXT("Name"), ArrayKeyIsName, ElementIsName, FNameFastLess());
	};

	UE_BENCHMARK(5, LookupByEnum);
	UE_BENCHMARK(5, LookupByString);
	UE_BENCHMARK(5, LookupByStringView);
	UE_BENCHMARK(5, LookupByName);

	UE_BENCHMARK(5, LookupByEnumFromStringView);
	UE_BENCHMARK(5, LookupByStringFromStringView);
	UE_BENCHMARK(5, LookupByNameFromStringView);

	UE_BENCHMARK(5, FindByEnum);
	UE_BENCHMARK(5, FindByString);
	UE_BENCHMARK(5, FindByStringView);
	UE_BENCHMARK(5, FindByName);

	UE_BENCHMARK(5, SearchByEnum);
	UE_BENCHMARK(5, SearchByString);
	UE_BENCHMARK(5, SearchByStringView);
	UE_BENCHMARK(5, SearchByName);

	return true;
}

/**
 * Compare various ways to check string views for equality with literals.
 * This was written for Madlibs code where this is a very common operation.
 */
IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FIGShowmeStringsBenchmark_StringViewEquals,
	"IGShowme.Strings.Benchmark.StringViewEquals",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::PerfFilter)
bool FIGShowmeStringsBenchmark_StringViewEquals::RunTest(const FString& Parameters)
{
	constexpr int32 NumOperations = 100000;

	// Compare a string view to a TEXT literal using op==.
	static const auto OpEqTEXT = [](const FStringView& Value) -> TOptional<ETextPluralForm> {
		if (Value == TEXT("zero"))
		{
			return ETextPluralForm::Zero;
		}
		else if (Value == TEXT("one"))
		{
			return ETextPluralForm::One;
		}
		else if (Value == TEXT("two"))
		{
			return ETextPluralForm::Two;
		}
		else if (Value == TEXT("few"))
		{
			return ETextPluralForm::Few;
		}
		else if (Value == TEXT("many"))
		{
			return ETextPluralForm::Many;
		}
		else if (Value == TEXT("other"))
		{
			return ETextPluralForm::Other;
		}
		else
		{
			return {};
		}
	};

	// Compare a string view to a string view (a TEXTVIEW literal) using op==.
	static const auto OpEqTEXTVIEW = [](const FStringView& Value) -> TOptional<ETextPluralForm> {
		if (Value == TEXTVIEW("zero"))
		{
			return ETextPluralForm::Zero;
		}
		else if (Value == TEXTVIEW("one"))
		{
			return ETextPluralForm::One;
		}
		else if (Value == TEXTVIEW("two"))
		{
			return ETextPluralForm::Two;
		}
		else if (Value == TEXTVIEW("few"))
		{
			return ETextPluralForm::Few;
		}
		else if (Value == TEXTVIEW("many"))
		{
			return ETextPluralForm::Many;
		}
		else if (Value == TEXTVIEW("other"))
		{
			return ETextPluralForm::Other;
		}
		else
		{
			return {};
		}
	};

	// Compare a string view to a TCHAR* using op==.
	static const auto OpEqTCHARPtr = [](const FStringView& Value) -> TOptional<ETextPluralForm> {
		constexpr const TCHAR* ZeroStr = TEXT("zero");
		constexpr const TCHAR* OneStr = TEXT("one");
		constexpr const TCHAR* TwoStr = TEXT("two");
		constexpr const TCHAR* FewStr = TEXT("few");
		constexpr const TCHAR* ManyStr = TEXT("many");
		constexpr const TCHAR* OtherStr = TEXT("other");
		if (Value == ZeroStr)
		{
			return ETextPluralForm::Zero;
		}
		else if (Value == OneStr)
		{
			return ETextPluralForm::One;
		}
		else if (Value == TwoStr)
		{
			return ETextPluralForm::Two;
		}
		else if (Value == FewStr)
		{
			return ETextPluralForm::Few;
		}
		else if (Value == ManyStr)
		{
			return ETextPluralForm::Many;
		}
		else if (Value == OtherStr)
		{
			return ETextPluralForm::Other;
		}
		else
		{
			return {};
		}
	};

	// Compare a string view to a TEXT literal using Equals (case sensitive).
	static const auto EqualsTEXTCS = [](const FStringView& Value) -> TOptional<ETextPluralForm> {
		if (Value.Equals(TEXT("zero"), ESearchCase::CaseSensitive))
		{
			return ETextPluralForm::Zero;
		}
		else if (Value.Equals(TEXT("one"), ESearchCase::CaseSensitive))
		{
			return ETextPluralForm::One;
		}
		else if (Value.Equals(TEXT("two"), ESearchCase::CaseSensitive))
		{
			return ETextPluralForm::Two;
		}
		else if (Value.Equals(TEXT("few"), ESearchCase::CaseSensitive))
		{
			return ETextPluralForm::Few;
		}
		else if (Value.Equals(TEXT("many"), ESearchCase::CaseSensitive))
		{
			return ETextPluralForm::Many;
		}
		else if (Value.Equals(TEXT("other"), ESearchCase::CaseSensitive))
		{
			return ETextPluralForm::Other;
		}
		else
		{
			return {};
		}
	};

	// Compare a string view to a string view (a TEXTVIEW literal) using Equals (case sensitive).
	static const auto EqualsTEXTVIEWCS = [](const FStringView& Value) -> TOptional<ETextPluralForm> {
		if (Value.Equals(TEXTVIEW("zero"), ESearchCase::CaseSensitive))
		{
			return ETextPluralForm::Zero;
		}
		else if (Value.Equals(TEXTVIEW("one"), ESearchCase::CaseSensitive))
		{
			return ETextPluralForm::One;
		}
		else if (Value.Equals(TEXTVIEW("two"), ESearchCase::CaseSensitive))
		{
			return ETextPluralForm::Two;
		}
		else if (Value.Equals(TEXTVIEW("few"), ESearchCase::CaseSensitive))
		{
			return ETextPluralForm::Few;
		}
		else if (Value.Equals(TEXTVIEW("many"), ESearchCase::CaseSensitive))
		{
			return ETextPluralForm::Many;
		}
		else if (Value.Equals(TEXTVIEW("other"), ESearchCase::CaseSensitive))
		{
			return ETextPluralForm::Other;
		}
		else
		{
			return {};
		}
	};

	static const auto RunTest = [](auto&& Comparer) {
		const FStringView Value = TEXTVIEW("wat");
		for (int32 i = 0; i < NumOperations; ++i)
		{
			Comparer(Value);
		}
	};

	// clang-format off
	const auto RunOpEqTEXT = []() { RunTest(OpEqTEXT); };
	const auto RunOpEqTEXTVIEW = []() { RunTest(OpEqTEXTVIEW); };
	const auto RunOpEqTCHARPtr = []() { RunTest(OpEqTCHARPtr); };
	const auto RunEqualsTEXTCS = []() { RunTest(EqualsTEXTCS); };
	const auto RunEqualsTEXTVIEWCS = []() { RunTest(EqualsTEXTVIEWCS); };
	// clang-format on

	// averages from a test on 2025-11-12
	UE_BENCHMARK(5, RunOpEqTEXT);         // 0.003509s
	UE_BENCHMARK(5, RunOpEqTEXTVIEW);     // 0.013878s
	UE_BENCHMARK(5, RunOpEqTCHARPtr);     // 0.005012s
	UE_BENCHMARK(5, RunEqualsTEXTCS);     // 0.002485s (case sensitive)
	UE_BENCHMARK(5, RunEqualsTEXTVIEWCS); // 0.011299s (case sensitive)

	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
