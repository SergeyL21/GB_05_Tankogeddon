// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelSaveGame.h"

#include "Objective.h"
#include "Quest.h"
#include "CollectionObjective.h"
#include "KillingObjective.h"
#include "InteractionObjective.h"
#include "Tankogeddon/BasePawn.h"

#include <functional>

#include "LocationObjective.h"

// --------------------------------------------------------------------------------------
ULevelSaveGame::ULevelSaveGame()
{
	Player = NewObject<UPawnSaveGame>();
}

// --------------------------------------------------------------------------------------
FString ULevelSaveGame::SerializeToJSON() const
{
	using json = nlohmann::json;
	json j_main, j_player;

	auto PawnToJSON = [](const UPawnSaveGame *Pawn, json& j)
	{
		if (Pawn && Pawn->BasePawnClass)
		{
			j["BasePawnClass"] = TCHAR_TO_UTF8(*Pawn->BasePawnClass->GetName());
			j["Health"] = {Pawn->CurrentHealth, Pawn->MaxHealth};
			j["Ammo"] = {Pawn->CurrentAmmo, Pawn->MaxAmmo};

			json j_transform;
			const auto Location {Pawn->Transform.GetLocation()};
			const auto Rotation {Pawn->Transform.GetRotation()};
			const auto Scale3D {Pawn->Transform.GetScale3D()};
			j_transform["Location"] = {Location.X, Location.Y, Location.Z};
			j_transform["Rotation"] = {Rotation.X, Rotation.Y, Rotation.Z, Rotation.W};
			j_transform["Scale"]= {Scale3D.X, Scale3D.Y, Scale3D.Z};
		
			j["Transform"] = j_transform;
		}
		return;
	};

	std::function<void(const AQuest*, json&)> QuestToJSON = [&](const AQuest* Quest, json& j)
	{
		if (Quest)
		{
			j["Name"] = TCHAR_TO_UTF8(*Quest->GetQuestName().ToString());
			j["Description"] = TCHAR_TO_UTF8(*Quest->GetQuestDescription().ToString());
			for (const auto Objective : Quest->GetObjectives())
			{
				json j_objective;
				j_objective["Description"] = TCHAR_TO_UTF8(*Objective->Description.ToString());
				j_objective["Type"] = TCHAR_TO_UTF8(*StaticEnum<EObjectiveType>()->GetValueAsString(Objective->Type));
				j_objective["IsCompleted"] = Objective->bIsCompleted;
				j_objective["CanBeCompleted"] = Objective->bCanBeCompleted;
				j["Objectives"].emplace_back(j_objective);
			}
			j["IsStoryQuest"] = Quest->IsStoryQuest();
			j["KeepObjectivesOrder"] = Quest->KeepObjectivesOrder();
			QuestToJSON(Quest->GetPrerequisiteQuest(), j["PrerequisiteQuest"]);
			j["IsTaken"] = Quest->IsTaken();
			j["Visibility"] = Quest->IsVisible();
		}
		
		return;	
	};

	PawnToJSON(Player, j_player);
	j_main["Player"] = j_player;
	for (const auto Enemy : Enemies)
	{
		json j_enemy;
		PawnToJSON(Enemy, j_enemy);
		j_main["Enemies"].emplace_back(j_enemy);
	}

	j_main["ActiveQuest"] = ActiveQuest == nullptr ? "NULL" : TCHAR_TO_UTF8(*ActiveQuest->Name.ToString());
	for (const auto Quest : Quests)
	{
		json j_quest;
		QuestToJSON(Quest, j_quest);
		j_main["Quests"].emplace_back(j_quest);
	}
	
	return j_main.dump(4).c_str();
}

// --------------------------------------------------------------------------------------
void ULevelSaveGame::DeserializeFromJSON(const FString& Text)
{
	using json = nlohmann::json;
	const json j_main {json::parse(TCHAR_TO_UTF8(*Text))};
	
	auto PawnFromJSON = [](UPawnSaveGame *Pawn, const json& j)
	{
		if (Pawn)
		{
			const auto j_health {j["Health"]};
			if (j_health.is_array() && j_health.size() > 1)
			{
				Pawn->CurrentHealth = j_health.at(0);
				Pawn->MaxHealth = j_health.at(1);
			}
			const auto j_ammo {j["Ammo"]};
			if (j_ammo.is_array() && j_ammo.size() > 1)
			{
				Pawn->CurrentAmmo = j_ammo.at(0);
				Pawn->MaxAmmo = j_ammo.at(1);
			}
			const auto j_transform {j["Transform"]};
			if (j_transform.is_object())
			{
				const auto j_location {j_transform["Location"]};
				if (j_location.is_array() && j_location.size() > 2)
				{
					Pawn->Transform.SetLocation(
						{j_location.at(0), j_location.at(1), j_location.at(2)});
				}

				const auto j_rotation {j_transform["Rotation"]};
				if (j_rotation.is_array() && j_rotation.size() > 3)
				{
					Pawn->Transform.SetRotation(
						{j_rotation.at(0), j_rotation.at(1), j_rotation.at(2), j_rotation.at(3)});
				}

				const auto j_scale {j_transform["Scale"]};
				if (j_scale.is_array() && j_scale.size() > 2)
				{
					Pawn->Transform.SetScale3D(
						{j_scale.at(0), j_scale.at(1), j_scale.at(2) });
				}
			}
		}
		return;
	};

	std::function<void(AQuest*, const json&)> QuestFromJSON = [&](AQuest* Quest, const json& j)
	{
		if (Quest)
		{
			const auto j_name {j["Name"]};
			if (j_name.is_string())
			{
				Quest->Name = FText::FromString(j_name.get<std::string>().c_str());
			}
			const auto j_desc {j["Description"]};
			if (j_desc.is_string())
			{
				Quest->Description = FText::FromString(j_desc.get<std::string>().c_str());
			}
			const auto j_objectives {j["Objectives"]};
			if (j_objectives.is_array())
			{
				Quest->Objectives.Empty();
				for (const auto j_objective : j_objectives)
				{
					FString StrType {j_objective["Type"].get<std::string>().c_str()};
					const auto EnumType {StaticEnum<EObjectiveType>()->GetValueByNameString(StrType)};
					
					UObjective *Objective {nullptr};
					switch (EnumType)
					{
					case EObjectiveType::Collect:	Objective = NewObject<UCollectionObjective>();	break;
					case EObjectiveType::Kill:		Objective = NewObject<UKillingObjective>();		break;
					case EObjectiveType::Interact:	Objective = NewObject<UInteractionObjective>();	break;
					case EObjectiveType::Location:	Objective = NewObject<ULocationObjective>();	break;
					default: break;
					}

					if (Objective)
					{
						Objective->Description = FText::FromString(j_objective["Description"].get<std::string>().c_str());
						Objective->Type = static_cast<EObjectiveType>(EnumType);
						Objective->bIsCompleted = j_objective["IsCompleted"].get<bool>();
						Objective->bCanBeCompleted = j_objective["CanBeCompleted"].get<bool>();
						Quest->Objectives.Add(Objective);
					}
				}
			}

			Quest->bIsStoryQuest = j["IsStoryQuest"].get<bool>();
			Quest->bKeepObjectivesOrder = j["KeepObjectivesOrder"].get<bool>();
			QuestFromJSON(Quest->GetPrerequisiteQuest(), j["PrerequisiteQuest"]);
			Quest->bIsTaken = j["IsTaken"].get<bool>();
			Quest->SetVisibility(j["Visibility"].get<bool>());
		}
		return;	
	};

	PawnFromJSON(Player, j_main["Player"]);
	Enemies.Empty();
	const auto j_enemies {j_main["Enemies"]};
	for (const auto j_enemy : j_enemies)
	{
		auto Enemy {NewObject<UPawnSaveGame>()};
		PawnFromJSON(Enemy, j_enemy);
		Enemies.Add(Enemy);
	}

	const auto j_active_quest {j_main["ActiveQuest"]};
	if (j_active_quest.is_string())
	{
		const auto JsonName {FText::FromString(j_active_quest.get<std::string>().c_str())};
		if (JsonName.CompareTo(FText::FromString("NULL")))
		{
			ActiveQuest = nullptr;
		}
		else if (ActiveQuest && ActiveQuest->Name.CompareTo(JsonName) > 0)
		{
			// ActiveQuest = FindQuestByName(JsonName);
		}
	}
	
	const auto j_quests {j_main["Quests"]};
	for (const auto j_quest : j_quests)
	{
		AQuest* Quest {nullptr};
		const FText JsonQuestName {FText::FromString(j_quest["Name"].get<std::string>().c_str())};
		for (auto OldQuest : Quests)
		{
			if (!OldQuest->Name.CompareTo(JsonQuestName))
			{
				Quest = OldQuest;
			}
		}
		
		QuestFromJSON(Quest, j_quest);
	}
	
	return;
}

