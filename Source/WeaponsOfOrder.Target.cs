using UnrealBuildTool;

public class WeaponsOfOrderTarget : TargetRules
{
	public WeaponsOfOrderTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("WeaponsOfOrder");
	}
}
