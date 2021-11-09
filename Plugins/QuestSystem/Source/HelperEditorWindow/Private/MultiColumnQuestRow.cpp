#include "MultiColumnQuestRow.h"

// --------------------------------------------------------------------------------------
TSharedRef<SWidget> SMultiColumnQuestRow::GenerateWidgetForColumn(const FName& InColumnName)
{
	if (InColumnName == "Actor_Name")
	{
		return SNew(SBorder)
			.Padding(1.f)
			.Content()
			[
				SNew(STextBlock)
				.ColorAndOpacity(FLinearColor{0.8f, 0.4f, 0.f, 1.f})
				.Text(FText::FromString(Item->GetName()))
			];
	}

	if (InColumnName == "Name")
	{
		return SNew(SBorder)
			.Padding(1.f)
			.Content()
			[
				SNew(SEditableText)
				.Text(Item->GetQuestName())
				.OnTextChanged_Lambda([this](const FText &Text)
				{
					Item->Name = Text;
				})
			];
	}

	if (InColumnName == "Description")
	{
		return 	SNew(SBorder)
			.Padding(1.f)
			.Content()
			[
				SNew(SEditableText)
				.Text(Item->GetQuestDescription())
				.OnTextChanged_Lambda([this](const FText &Text)
				{
					Item->Description = Text;
				})
				
			];
	}

	if (InColumnName == "IsStoryQuest")
	{
		return SNew(SBorder)
			.HAlign(HAlign_Center)
			.Padding(1.f)
			.Content()
			[
				SNew(SCheckBox)
				.IsChecked(Item->IsStoryQuest() ? ECheckBoxState::Checked : ECheckBoxState::Unchecked)
				.OnCheckStateChanged_Lambda([this](ECheckBoxState State)
				{
					Item->bIsStoryQuest = (State == ECheckBoxState::Checked ? true : false);
				})
			];
	} 
	
	return SNullWidget::NullWidget;
}

// --------------------------------------------------------------------------------------
void SMultiColumnQuestRow::Construct(const FArguments& InArgs,
	const TSharedRef<STableViewBase>& InOwnerTableView)
{
	Item = InArgs._Item;
	FSuperRowType::Construct(FSuperRowType::FArguments(), InOwnerTableView);
	return;
}