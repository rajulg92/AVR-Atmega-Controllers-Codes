#define SharpOnRail 0
#define SharpOnBot 3
#define BotSharpClose	100
#define RailSharpClose 100

void RailUp2Bars(void)
{
	while (GetSharpInput(SharpOnRail) != RailSharpClose)
	{
		Rail_Up();
	}
	_delay_ms(300);
	while (GetSharpInput(SharpOnRail) != RailSharpClose)
	{
		Rail_Up();
	}
	All_Motors_stop();
}

void BotUp2Bars(void)
{
	while (GetSharpInput(SharpOnBot) != BotSharpClose)
	{
		Rail_Down();
	}
	_delay_ms(300);
	while (GetSharpInput(SharpOnBot) != BotSharpClose)
	{
		Rail_Down();
	}
	All_Motors_stop();
}	

void JungleGym(void)
{
	while (GetSharpInput(SharpOnBot) != BotSharpClose)					
	{
		Rail_Down();
	}
	All_Motors_stop();
	
}