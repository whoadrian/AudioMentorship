// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class Mentorship_01_ScifiBaseEditorTarget : TargetRules
{
	public Mentorship_01_ScifiBaseEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V4;

		ExtraModuleNames.AddRange( new string[] { "Mentorship_01_ScifiBase" } );
	}
}
