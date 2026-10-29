#include "common/rulesys.h"
#include "common/skill_caps.h"
#include "zone/client.h"

// Skill cap for this character's class at a level (multiclass servers take the highest of their classes)
uint16 Client::GetHighestClassSkillCap(EQ::skills::SkillType skill, uint8 level)
{
	return SkillCaps::Instance()->GetSkillCap(GetClass(), skill, level).cap;
}

// Level-up and login: skills (not tradeskills) to their caps for the current level, plus every spell and
// discipline the character's class has up to it. Only raises / adds, so it's quiet once caught up.
void Client::AutoLearnForLevel()
{
	const uint8 level = GetLevel();

	if (RuleB(Custom, AutoTrainSkills)) {
		int trained = 0;
		for (const auto &s : EQ::skills::GetSkillTypeMap()) {
			if (EQ::skills::IsTradeskill(s.first)) {
				continue;
			}

			uint16 cap = GetHighestClassSkillCap(s.first, level);
			if (EQ::skills::IsSpecializedSkill(s.first)) {
				// What a guildmaster would train; going past it is still earned by casting
				cap = std::min<uint16>(cap, RuleI(Skills, MaxTrainSpecializations));
			}

			if (GetRawSkill(s.first) < cap) {
				SetSkill(s.first, cap);
				trained++;
			}
		}

		if (trained) {
			Message(Chat::White, fmt::format("Your skills have been trained to their limits for level {}.", level).c_str());
		}
	}

	if (RuleB(Custom, AutoScribeSpells)) {
		ScribeSpells(1, level);
	}

	if (RuleB(Custom, AutoLearnDisciplines)) {
		LearnDisciplines(1, level);
	}
}
