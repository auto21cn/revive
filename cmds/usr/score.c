// score.c

#include <ansi.h>
#include <combat.h>

inherit F_CLEAN_UP;

string bar_string = "■■■■■■■■■■■■■■■■■■■■■■■■■";
string blank_string = "□□□□□□□□□□□□□□□□□□□□□□□□□";

string display_attr(int gift, int value);
string status_color(int current, int max);
string date_string(int date);
string tribar_graph(int val, int eff, int max, string color);

mapping oprank = ([
        "宗师"     : HIW "★★★★★",
        "登峰造极" : HIY "★★★★☆",
        "超凡脱俗" : HIY "★★★★  ",
        "臻至化境" : HIR "★★★☆  ",
        "炉火纯青" : HIR "★★★    ",
        "已有大成" : HIC "★★☆    ",
        "非同凡响" : HIC "★★      ",
        "出类拔萃" : HIG "★☆      ",
        "身手不凡" : HIG "★        ",
        "已有小成" : HIM "☆        ",
]);

void create() { seteuid(ROOT_UID); }

int main(object me, string arg)
{
	object ob;
	mapping my;
        mapping mci;
        int age, month;
	string line, str, skill_type;
	object weapon;
	int attack_points, dodge_points, parry_points;

	seteuid(getuid(me));

	if (! arg)
		ob = me;
	else if (wizardp(me))
        {
		ob = present(arg, environment(me));
		if (! ob || ! ob->is_character()) ob = find_player(arg);
		if (! ob || ! ob->is_character()) ob = find_living(arg);
		if (! ob || ! ob->is_character())
                        return notify_fail("你要察看谁的状态？\n");
	} else
		return notify_fail("只有巫师能察看别人的状态。\n");

	my = ob->query_entire_dbase();
        mci = my["combat"];
        if (! mci) mci = ([ ]);

        if (playerp(ob) && (! stringp(my["born"]) || ! my["born"]))
                return notify_fail("还没有出生呐，察看什么？\n");

        if (my["max_jing"] < 1 || my["max_qi"] < 1)
                return notify_fail("无法察看" + ob->name(1) + "的状态。\n");

	line = sprintf( BOLD "%s" NOR "%s\n\n", RANK_D->query_rank(ob), ob->short(1) );
	if (playerp(ob))
        {
                age = ob->query("age");
                month = ob->query("mud_age");
                if (age >= 60)
                {
                        month -= (age - 60) * 86400 * 4;
                        month = (month - 118 * 86400) / 7200 / 4 + 1;
                } else
                if (age >= 30)
                {
                        month -= (age - 30) * 86400 * 3;
                        month = (month - 28 * 86400) / 7200 / 3 + 1;
                } else
                if (age >= 18)
                {
                        month -= (age - 18) * 86400 * 2;
                        month = (month - 4 * 86400) / 7200 / 2 + 1;
                } else
                {
                        month -= (age - 14) * 86400;
                        month = month / 7200 + 1;
                }

                // not show invalid month
                if (month > 12 || month < 1)
                        month = 1;

        	line += sprintf(WHT " 你是一%s%s岁%s个月的%s%s，%s生。\n",
        		ob->query("unit"),
        		chinese_number(age), 
        		chinese_number(month), 
        		ob->query("gender"),
        		ob->query("race"),
        		CHINESE_D->chinese_date(ob->query("birthday")));
        }
	else
	line += sprintf(WHT " 你是一%s%s岁的%s%s，%s生。\n",
		ob->query("unit"),
		chinese_number(ob->query("age")), 
		ob->query("gender"),
		ob->query("race"),
		CHINESE_D->chinese_date(ob->query("birthday")));

	line += sprintf(
		WHT " 膂力：[%s]  悟性：[%s]  根骨：[%s]  身法：[%s]\n\n", 
		display_attr(my["str"], ob->query_str()) + WHT,
		display_attr(my["int"], ob->query_int()) + WHT,
		display_attr(my["con"], ob->query_con()) + WHT,
		display_attr(my["dex"], ob->query_dex()) + WHT);
	if (playerp(ob))
	{
                if (my["born_family"] && my["born_family"] != "没有")
		{
                        line += HIW " 你出生在" + my["born_family"] +
				"，天性" + my["character"];
                } else
                if (stringp(my["born"]) && my["born"])
		{
                        line += HIW " 你是" + my["born"] +
				"，天性" + my["character"];
                } else
                        line += HIW " 你尚未出生";

		if (mapp(my["family"])) {
			if (my["family"]["master_name"])
				line += sprintf("，师父是%s。\n" NOR,
					        my["family"]["master_name"]);
                        else
                                line += "，还没有师父。\n" NOR;
		} else
                        line += "，还没有拜师。\n" NOR;

		if ((int)ob->query("balance") > 0)
			line += HIY " 你目前的存款：" + MONEY_D->money_str((int)ob->query("balance")) + "。\n" NOR;
		else
			line += HIY " 你目前没有存款。\n" NOR;

		if (! ob->query("couple/name"))
		{
			if (ob->query("gender") == "女性")
				line += HIC " 你尚未婚配。\n" NOR;
			else
			        line += HIC " 你尚未娶妻。\n" NOR;
		}
		else
			line += HIC " 你的伴侣是" + ob->query("couple/name") + "(" +
                                ob->query("couple/id") + ")。\n" NOR;

                if (! (int)ob->query("sex/times"))
                {
                        if (ob->query("gender") == "女性")
                                line += HIG " 你还是处女。\n" NOR;
                        else
                                line += HIC " 你还是童男。\n" NOR;
                } else
                {
                        string *ks;

                        ks = keys(ob->query("sex"));
                        ks -= ({ "times", "first", "" });
                        if (sizeof(ks) >= 10)
                                line += HIW " 你曾经和数不清的人发生过关系，结果"
                                        "连自己都忘了有谁了。\n" NOR;
                        else
                                line += HIW " 你曾经和" + implode(ks, HIW "、") +
                                        HIW "发生过关系。\n" NOR;
                }

                line += "\n";
	}

	line += WHT " <精>  " + tribar_graph(my["jing"], my["eff_jing"], my["max_jing"], GRN) + "\n";
	line += WHT " <气>  " + tribar_graph(my["qi"], my["eff_qi"], my["max_qi"], HIR) + "\n";

	if (objectp(weapon = ob->query_temp("weapon")))
        {
		skill_type = weapon->query("skill_type");
                attack_points = COMBAT_D->skill_power(ob, skill_type, SKILL_USAGE_ATTACK);
	} else
        {
                mapping prepare;
                string *sk;

                prepare = me->query_skill_prepare();
                if (!prepare) prepare = ([]);
                sk = keys(prepare);
        
                if ( sizeof(sk) == 0 ) skill_type = "unarmed"; else
                                       skill_type = sk[0];
                attack_points = COMBAT_D->skill_power(ob, skill_type, SKILL_USAGE_ATTACK);
                if ( sizeof(prepare) == 2 )
                {
                        int temp;
                        temp = COMBAT_D->skill_power(ob, sk[1], SKILL_USAGE_ATTACK);
                        if (attack_points < temp) attack_points = temp;
                }
        }

	parry_points = COMBAT_D->skill_power(ob, "parry", SKILL_USAGE_DEFENSE);
	dodge_points = COMBAT_D->skill_power(ob, "dodge", SKILL_USAGE_DEFENSE);

        if (wizardp(me))
        {
	        line += sprintf(WHT "\n 战斗攻击力 " HIW "%8d" NOR WHT "\t\t战斗防御力 " HIW "%8d\n" NOR,
		        attack_points/100 + 1, (dodge_points + (weapon? parry_points: (parry_points/10)))/100 + 1,);
	        line += sprintf(WHT " 战斗伤害力 " HIW "%8d" NOR WHT "\t\t战斗保护力 " HIW "%8d\n" NOR,
		        weapon ? ob->query_temp("apply/damage")
                               : ob->query_temp("apply/unarmed_damage"),
                        ob->query_temp("apply/armor"));
	}
        line += "\n";

	line += WHT " 食物：" + tribar_graph(my["food"], ob->max_food_capacity(), ob->max_food_capacity(), YEL) + "\n";
	line += WHT " 饮水：" + tribar_graph(my["water"], ob->max_water_capacity(), ob->max_water_capacity(), CYN) + "\n";

        if (! ((int)mci["MKS"] + (int)mci["PKS"]))
        {
                line += HIY "\n 你到目前为止尚未开杀戒。\n";
        } else
        {
                line += sprintf(HIR "\n 你总共杀了%s次人", chinese_number(mci["MKS"] + mci["PKS"]));
                if ((int)mci["PKS"])
                {
                        line += "，其中" + chinese_number(mci["PKS"]) + "次是玩家";
                        if ((int)mci["WPK"] >= (int)mci["PKS"])
                        {
                                mci["WPK"] = (int)mci["PKS"];
                                line += "，全都是你有意杀害的。\n";
                        } else
                        if ((int)mci["WPK"])
                        {
                                line += "，" + chinese_number(mci["WPK"]) +
                                        "次是你有意杀害的。\n";
                        } else
                                line += "，他们都是你无心杀害的。\n";
                }
                else
                        line += "。\n";
        }
        if ((int)mci["DPS"])
        {
                line += sprintf(HIR "\n 你总共打晕了%s次玩家", chinese_number(mci["DPS"]));
                if ((int)mci["DPS_GOOD"] >= (int)mci["DPS"])
                        line += "，他们全部都是正派人士";
                else
                {
                        if ((int)mci["DPS_GOOD"])
                                line += "，其中" + chinese_number(mci["DPS_GOOD"]) + "人是正派人士";

                        if ((int)mci["DPS_BAD"] >= (int)mci["DPS"])
                                line += "，他们全部都是邪派人士";
                        else
                        if ((int)mci["DPS_BAD"])
                                line += "，" + ((int)mci["DPS_GOOD"] ? "" : "其中") +
                                        chinese_number(mci["DPS_BAD"]) + "人是邪派人士";
                }
                line += "。\n";
        }
        line += "\n" NOR;

        if ((int)mci["dietimes"])
        {
	        line += sprintf(HIR " 你到目前为止总共到黑白无常那里"
				"串门%s次。\n" NOR,
				chinese_number(mci["dietimes"]));
		if (stringp(mci["last_die"]))
			line += sprintf(HIR " 你最后一次是%s。\n" NOR,
					mci["last_die"]);
		line += "\n";
        }

        while (playerp(ob))
        {
                string op;
                if (ultrap(ob))
                {
                        line += HIC " 你现在已经成为武学大宗师。\n\n" NOR;
                        break;
                }

                line += WHT " 拳脚功夫：" NOR;
                if (op = ob->query("opinion/unarmed"))
                        line += oprank[op] + "        ";
                else
                        line += CYN "无评价            ";

                line += NOR WHT "    兵器运用：" NOR;
                if (op = ob->query("opinion/weapon"))
                        line += oprank[op] + "        ";
                else
                        line += CYN "无评价";

                line += NOR WHT "\n 内家功夫：" NOR;
                if (op = ob->query("opinion/force"))
                        line += oprank[op] + "        ";
                else
                        line += CYN "无评价            ";

                line += NOR WHT "    轻身功夫：" NOR;
                if (op = ob->query("opinion/dodge"))
                        line += oprank[op] + "        ";
                else
                        line += CYN "无评价";

                line += "\n\n" NOR;
                break;
        }

        if (my["breakup"])
        {
                line += HIY " 你已经打通了任督二脉";
                if (my["animaout"])
                        line += "、修成了元婴出世";
                line += "，可以窥测更高深的武学境界。\n\n";
        }

	line += sprintf(WHT " 实战经验： " HIM "%8d\t\t" NOR, my["combat_exp"]);
	line += sprintf(WHT " 实战体会： %s%8d\n" NOR,
                        my["combat_exp"] < 100000 ? HIR :
                        my["experience"] < ob->query_experience_limit() ? HIY : HIM,
                        (my["experience"] - my["learned_experience"]));
	if (playerp(ob))
        {
                line += sprintf(WHT " 江湖阅历： " HIC "%8d\t\t" NOR, my["score"]);
	        line += sprintf(WHT " 江湖威望： " HIC "%8d\n" NOR, my["weiwang"]);
        }
	if (my["shen"] >=0)
		line += sprintf(WHT " 正    气： " WHT "%8d\t\t" NOR, my["shen"]);
	else
		line += sprintf(WHT " 邪    气： " RED "%8d\t\t" NOR, -ob->query("shen"));
	line += sprintf(HIW " 戾    气： " HIR "%8d\n" NOR, my["total_hatred"]);

	write(line);
	return 1;
}

string display_attr(int gift, int value)
{
	if( value > gift ) return sprintf( HIY "%3d" NOR, value );
	else if( value < gift ) return sprintf( CYN "%3d" NOR, value );
	else return sprintf("%3d", value);
}

string status_color(int current, int max)
{
	int percent;

	if (max) percent = current * 100 / max;
	else percent = 100;

	if (percent > 100) return HIC;
	if (percent >= 90) return HIG;
	if (percent >= 60) return HIY;
	if (percent >= 30) return YEL;
	if (percent >= 10) return HIR;
	return RED;
}

string tribar_graph(int val, int eff, int max, string color)
{
	return color + bar_string[0..(val * 25 / max) * 2 - 1]
		+ ((eff > val) ? blank_string[(val * 25 / max) * 2..(eff * 25 / max) * 2 - 1] : "") + NOR;
}

int help(object me)
{
	write(@HELP
指令格式 : score
           score <对象名称>                   (巫师专用)

这个指令可以显示你(你)或指定对象(含怪物)的基本资料。
基本资料的设定请参阅 'help setup'。

see also : hp
HELP );
    return 1;
}
