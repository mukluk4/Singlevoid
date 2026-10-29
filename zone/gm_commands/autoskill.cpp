#include "../client.h"
#include "../command.h"
#include "../../common/rulesys.h"
#include "../../common/strings.h"

// #autoskill: pick combat abilities that fire on their own during auto-attack whenever they are ready
// (Client::ProcessAutoSkills). Each choice is a character data bucket, autoskill.<skill id>.
namespace {
	enum class AutoSkillMode { Status, Enable, Disable };

	// Last argument: enable / disable / status, or none (status)
	bool ParseMode(const std::string &arg, AutoSkillMode &mode)
	{
		const auto a = Strings::ToLower(arg);
		if (a == "enable" || a == "on" || a == "true" || a == "1" || a == "yes") {
			mode = AutoSkillMode::Enable;
			return true;
		}
		if (a == "disable" || a == "off" || a == "false" || a == "0" || a == "no") {
			mode = AutoSkillMode::Disable;
			return true;
		}
		if (a == "status") {
			mode = AutoSkillMode::Status;
			return true;
		}
		mode = AutoSkillMode::Status;
		return false;
	}

	// Skill id, or a name with or without spaces ("flying kick", "flyingkick")
	int FindAutoSkill(const std::string &input)
	{
		if (Strings::IsNumber(input)) {
			const int id = Strings::ToInt(input);
			for (const auto s : Client::GetAvailableAutoSkills()) {
				if (static_cast<int>(s) == id) {
					return id;
				}
			}
			return -1;
		}

		auto wanted = Strings::ToLower(input);
		Strings::FindReplace(wanted, " ", "");
		for (const auto s : Client::GetAvailableAutoSkills()) {
			auto name = Strings::ToLower(EQ::skills::GetSkillName(s));
			Strings::FindReplace(name, " ", "");
			if (name == wanted) {
				return static_cast<int>(s);
			}
		}
		return -1;
	}
}

void command_autoskill(Client *c, const Seperator *sep)
{
	if (!RuleB(Custom, AutoSkill)) {
		c->Message(Chat::White, "#autoskill is not enabled on this server.");
		return;
	}

	const std::string usage = "Usage: #autoskill [skill name or id] [on|off|status], or #autoskill list";
	if (sep->argnum < 1) {
		c->Message(Chat::Skills, usage.c_str());
		return;
	}

	if (Strings::ToLower(sep->arg[1]) == "list") {
		c->Message(Chat::Skills, "Combat abilities you can set to fire on their own:");
		bool any = false;
		for (const auto s : c->GetAutoSkillsList()) {
			any = true;
			c->Message(
				Chat::Skills,
				fmt::format(
					"{} ({}) - {}",
					EQ::skills::GetSkillName(s),
					static_cast<int>(s),
					c->GetAutoSkillStatus(s) ? "on" : "off"
				).c_str()
			);
		}
		if (!any) {
			c->Message(Chat::Skills, "You have no combat abilities that can be automated.");
		}
		return;
	}

	AutoSkillMode mode;
	const bool has_mode = ParseMode(sep->arg[sep->argnum], mode);
	const int  last     = has_mode ? sep->argnum - 1 : sep->argnum;
	if (last < 1) {
		c->Message(Chat::Skills, usage.c_str());
		return;
	}

	std::string skill_name;
	for (int i = 1; i <= last; i++) {
		if (i > 1) {
			skill_name += " ";
		}
		skill_name += sep->arg[i];
	}

	const int skill_id = FindAutoSkill(skill_name);
	if (skill_id < 0) {
		c->Message(Chat::Skills, fmt::format("'{}' is not a combat ability #autoskill can use.", skill_name).c_str());
		c->Message(Chat::Skills, usage.c_str());
		return;
	}

	const auto skill = static_cast<EQ::skills::SkillType>(skill_id);
	if (!c->CanHaveSkill(skill)) {
		c->Message(Chat::Skills, fmt::format("Your class cannot use {}.", EQ::skills::GetSkillName(skill)).c_str());
		return;
	}

	switch (mode) {
		case AutoSkillMode::Enable:
			c->SetAutoSkillStatus(skill, true);
			c->Message(Chat::Skills, fmt::format("{} will now fire on its own during auto-attack.", EQ::skills::GetSkillName(skill)).c_str());
			break;
		case AutoSkillMode::Disable:
			c->SetAutoSkillStatus(skill, false);
			c->Message(Chat::Skills, fmt::format("{} is back to manual.", EQ::skills::GetSkillName(skill)).c_str());
			break;
		default:
			c->Message(
				Chat::Skills,
				fmt::format("{} automatic use is {}.", EQ::skills::GetSkillName(skill), c->GetAutoSkillStatus(skill) ? "on" : "off").c_str()
			);
			break;
	}
}
