#pragma once

#include <CoreMinimal.h>
#include "Quest.h"

class SMultiColumnQuestRow : public SMultiColumnTableRow<AQuest*>
{
public:

	SLATE_BEGIN_ARGS(SMultiColumnQuestRow) {}
	SLATE_ARGUMENT(AQuest*, Item)
	SLATE_END_ARGS()
		
public:
	virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& InColumnName) override;

	void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView);
	
private:
	AQuest *Item {nullptr};
};
