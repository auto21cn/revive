// story:dabei

#include <ansi.h>

inherit F_DBASE;

int     give_gift();
mixed   continue_story(int n);

// 外部可以调用的函数
void    start_fight(string result, object who);
void    stop_story(string result);

static mixed *story = ({
        "叮呤几下兵器相交之声传了过来。",
        "谢烟客心中一凛，托起石破天，几个起落之后，已经到了一棵大树之后。",
        "两人在树后望去，只见林中有四人相斗，乃是三人夹攻一人。",
        "谢烟客凝神一看，原来被围攻的却是白鲸岛主大悲老人，另外三人却不认得，只是武功非同泛泛。",
        "只见大悲老人已然不敌，身上负伤数处，数滴献血飞溅出来。",
        (: give_gift :),
});

void create()
{
        seteuid(getuid());
}

string prompt() { return HIR "【武林传闻】" NOR; }

mixed query_story_message(int step)
{
        return step < sizeof(story) ? story[step] :
                                      continue_story(step - sizeof(story));
}

mixed continue_story(int n)
{
        object *obs;

        if (! arrayp(obs = query_temp("actor")) ||
            ! objectp(obs[0]) || ! objectp(obs[1]))
        {
                // story can not continue
                stop_story(0);
                return 0;
        }

        return 1;
}

void start_fight(string result, object who)
{
        string msg;

        if (query_temp("fighter"))
                return;

        switch (result)
        {
        case "dabei":
                msg = "听说$N出手和长乐帮众一起围攻大悲老人。";
                break;

        case "mi":
                msg = "听说$N挺身而出，和大悲老人一同反击长乐帮众。";
                break;

        default:
                return;
        }

        msg = replace_string(msg, "$N", who->name(1) + HIM);
        set_temp("fighter", who);
        set_temp("start_fight", result);
        CHANNEL_D->do_channel(this_object(), "rumor", msg);
}

void stop_story(string result)
{
        object *obs;
        object aob;

        switch (result)
        {
        case "dabei die":
                // 大悲老人被打死
                break;

        case "mi die":
                // 米香主被打死
                break;
        default:
                CHANNEL_D->do_channel(this_object(), "rumor",
                        "听说长乐帮众击毙了白鲸岛主大悲老人，又做下一场武林血案。");
                break;
        }

        obs = query_temp("actor");
        if (arrayp(obs))
        {
                obs -= ({ 0 });
                foreach (aob in obs)
                {
                        if (! living(aob)) aob->revive();
                        message_vision("$N见状大吃一惊，赶紧转身逃走了。\n", aob);
                        destruct(aob);
                }
        }
}

int give_gift()
{
        object dabei, mi, dizi1, dizi2;

        dabei  = new("/d/city/npc/dabei");
        mi     = new("/d/city/npc/mi");
        dizi1  = new("/d/city/npc/cldizi");
        dizi2  = new("/d/city/npc/cidizi");

        mi->set_temp("helper", ({ dizi1, dizi2 }));
        mi->set_temp("story", this_object());
        dabei->set_temp("story", this_object());

        set_temp("actor", ({ dabei, mi, dizi1, dizi2 }));

        return 1;
}
