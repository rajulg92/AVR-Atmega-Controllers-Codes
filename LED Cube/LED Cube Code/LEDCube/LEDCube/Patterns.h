#include <stdlib.h>
#define del 15
#define InOutdel 10
#define fall_dell 15
#define PlaneShiftDel 10
#define FillCubedel 40
#define OneByOneLEDOndel 10
#define DrawLinedel 100
int cnt = 0;
void Test_all_LEDs()
{
	for(int j = 0 ; j< 3;j++)
	{
		for (int i = 1; i <=9; i++)
		{
			for (int cnt = 0; cnt < del*10; cnt++)
			{
				LED_On(j,i);
			}
		}
		
	}
}

void Rectangle_loop()
{	
	for (int cnt = 0; cnt < del; cnt++)
	{
		LED_hor_plane_on(1);
	}
	
	
	for (int cnt = 0; cnt < del; cnt++)
	{
		LED_On(0,1);
		LED_On(0,2);
		LED_On(0,3);
		LED_On(1,4);
		LED_On(1,5);
		LED_On(1,6);
		LED_On(2,7);
		LED_On(2,8);
		LED_On(2,9);
	}
	
	for (int cnt = 0; cnt < del; cnt++)
	{
		LED_On(0,4);
		LED_On(0,5);
		LED_On(0,6);
		LED_On(1,4);
		LED_On(1,5);
		LED_On(1,6);
		LED_On(2,4);
		LED_On(2,5);
		LED_On(2,6);
	}
	
	for (int cnt = 0; cnt < del; cnt++)
	{
		LED_On(0,7);
		LED_On(0,8);
		LED_On(0,9);
		LED_On(1,4);
		LED_On(1,5);
		LED_On(1,6);
		LED_On(2,1);
		LED_On(2,2);
		LED_On(2,3);
	}

}

void Fall()
{
	cnt = 0;
	while (cnt<fall_dell)
	{
		LED_hor_plane_on(0);
		cnt++;
	}
	cnt = 0;
	while (cnt<fall_dell)
	{
		LED_hor_plane_on(1);
		cnt++;
	}
	cnt = 0;
	for (int i = 0;i<100;i++)
	{
		LED_hor_plane_on(2);
	}
	
//--------------------------------------	
	while (cnt<fall_dell)
	{
		LED_On(1,9);
		for (int i = 0; i < 9; i ++)
		{
			LED_On(2,i);
			
		}
		cnt++;
		
	}
	cnt = 0;
	while (cnt<fall_dell)
	{
		for (int i = 0; i < 9; i ++)
		{
			LED_On(2,i);
		}
		LED_On(0,9);										//**
		cnt++;
	}
	cnt = 0;

//--------------------------------------
while (cnt<fall_dell)
{
	LED_On(0,9);
	LED_On(1,8);
	for (int i = 0; i < 8; i ++)
	{
		LED_On(2,i);
		
	}
	cnt++;
	
}
cnt = 0;
while (cnt<fall_dell)
{
	for (int i = 0; i < 8; i ++)
	{
		LED_On(2,i);
	}
	LED_On(0,9);
	LED_On(0,8);											//**
	cnt++;
}
cnt = 0;
//--------------------------------------
while (cnt<fall_dell)
{
	LED_On(1,7);
	LED_On(0,9);
	LED_On(0,8);
	for (int i = 0; i < 7; i ++)
	{
		LED_On(2,i);
		
	}
	cnt++;
	
}
cnt = 0;
while (cnt<fall_dell)
{
	for (int i = 0; i < 7; i ++)
	{
		LED_On(2,i);
	}
	LED_On(0,9);
	LED_On(0,8);
	LED_On(0,7);
	cnt++;
}
cnt = 0;
//--------------------------------------
while (cnt<fall_dell)
{
	LED_On(1,6);
	LED_On(0,9);
	LED_On(0,8);
	LED_On(0,7);
	for (int i = 0; i < 6; i ++)
	{
		LED_On(2,i);
		
	}
	cnt++;
	
}
cnt = 0;
while (cnt<fall_dell)
{
	for (int i = 0; i < 6; i ++)
	{
		LED_On(2,i);
	}
	LED_On(0,9);
	LED_On(0,8);
	LED_On(0,7);
	LED_On(0,6);
	
	cnt++;
}
cnt = 0;
//--------------------------------------
while (cnt<fall_dell)
{
	LED_On(1,5);
	LED_On(0,9);
	LED_On(0,8);
	LED_On(0,7);
	LED_On(0,6);
	for (int i = 0; i < 5; i ++)
	{
		LED_On(2,i);
		
	}
	cnt++;
	
}
cnt = 0;
while (cnt<fall_dell)
{
	for (int i = 0; i < 5; i ++)
	{
		LED_On(2,i);
	}
	LED_On(0,9);
	LED_On(0,8);
	LED_On(0,7);
	LED_On(0,6);
	LED_On(0,5);
	cnt++;
}
cnt = 0;
//--------------------------------------
while (cnt<fall_dell)
{
	LED_On(1,4);
	LED_On(0,9);
	LED_On(0,8);
	LED_On(0,7);
	LED_On(0,6);
	LED_On(0,5);
	for (int i = 0; i < 4; i ++)
	{
		LED_On(2,i);
		
	}
	cnt++;
	
}
cnt = 0;
while (cnt<fall_dell)
{
	for (int i = 0; i < 4; i ++)
	{
		LED_On(2,i);
	}
	LED_On(0,9);
	LED_On(0,8);
	LED_On(0,7);
	LED_On(0,6);
	LED_On(0,5);
	LED_On(0,4);
	cnt++;
}
cnt = 0;
//--------------------------------------
while (cnt<fall_dell)
{
	LED_On(1,3);
	LED_On(0,9);
	LED_On(0,8);
	LED_On(0,7);
	LED_On(0,6);
	LED_On(0,5);
	LED_On(0,4);
	for (int i = 0; i < 3; i ++)
	{
		LED_On(2,i);
		
	}
	cnt++;
	
}
cnt = 0;
while (cnt<fall_dell)
{
	for (int i = 0; i < 3; i ++)
	{
		LED_On(2,i);
	}
	LED_On(0,9);
	LED_On(0,8);
	LED_On(0,7);
	LED_On(0,6);
	LED_On(0,5);
	LED_On(0,4);
	LED_On(0,3);
	cnt++;
}
cnt = 0;
//--------------------------------------
while (cnt<fall_dell)
{
	LED_On(1,2);
	LED_On(0,9);
	LED_On(0,8);
	LED_On(0,7);
	LED_On(0,6);
	LED_On(0,5);
	LED_On(0,4);
	LED_On(0,3);
	for (int i = 0; i < 2; i ++)
	{
		LED_On(2,i);
		
	}
	cnt++;
	
}
cnt = 0;
while (cnt<fall_dell)
{
	for (int i = 0; i < 2; i ++)
	{
		LED_On(2,i);
	}
	LED_On(0,9);
	LED_On(0,8);
	LED_On(0,7);
	LED_On(0,6);
	LED_On(0,5);
	LED_On(0,4);
	LED_On(0,3);
	LED_On(0,2);
	cnt++;
}
cnt = 0;

//--------------------------------------
while (cnt<fall_dell)
{
	LED_On(1,1);
	LED_On(0,9);
	LED_On(0,8);
	LED_On(0,7);
	LED_On(0,6);
	LED_On(0,5);
	LED_On(0,4);
	LED_On(0,3);
	LED_On(0,2);
	for (int i = 0; i < 1; i ++)
	{
		LED_On(2,i);
		
	}
	cnt++;
	
}
cnt = 0;
while (cnt<fall_dell*10)
{
	for (int i = 0; i < 1; i ++)
	{
		LED_On(2,i);
	}
	LED_On(0,9);
	LED_On(0,8);
	LED_On(0,7);
	LED_On(0,6);
	LED_On(0,5);
	LED_On(0,4);
	LED_On(0,3);
	LED_On(0,2);
	LED_On(0,1);
	cnt++;
}
cnt = 0;

}

void PlaneShift()
{
	int i,j;
	cnt = 0;
	for (j = 0; j < 5; j++)
	{
		for (i = 0; i < 3; i++)
		{
			while(cnt<PlaneShiftDel)
			{
				LED_hor_plane_on(i);
				cnt++;
			}
			cnt = 0;
		}
		
		while(cnt<PlaneShiftDel)
		{
			LED_hor_plane_on(1);
			cnt++;
		}
		cnt = 0;
	}
	
	
	for (j = 0; j < 5; j++)
	{
		while(cnt<PlaneShiftDel)
		{
			for(int k = 0;k < 3; k++) for (i = 1;i < 5;i++)	LED_On(k,i);
			cnt++;
		}
		cnt = 0;
		
		while(cnt<PlaneShiftDel)
		{
			for(int k = 0;k < 3; k++) for (i = 4;i < 7;i++)	LED_On(k,i);
			cnt++;
		}
		cnt = 0;
		while(cnt<PlaneShiftDel)
		{
			for(int k = 0;k < 3; k++) for (i = 7;i < 10;i++)	LED_On(k,i);
			cnt++;
		}
		cnt = 0;
		while(cnt<PlaneShiftDel)
		{
			for(int k = 0;k < 3; k++) for (i = 4;i < 7;i++)	LED_On(k,i);
			cnt++;
		}
		cnt = 0;
	}
}

void Glow_Random()
{
	int k,j;
	for (int i = 1; i < 200; i++)
	{
		k = rand()%3;
		j = rand()%10;
		for (int cnt = 0; cnt < del*2; cnt++)
		{
			LED_On(k,j);
			
		}
		
	}
}

void FillCube()
{
	for (cnt = 0;cnt< FillCubedel*3; cnt++)
	{
		LED_On(2,9);
	}
	for (cnt = 0;cnt< FillCubedel/2; cnt++)
	{
		LED_On(2,9);	LED_On(2,5);	LED_On(2,6);	LED_On(2,8);
		LED_On(1,9);	LED_On(1,5);	LED_On(1,6);	LED_On(1,8);
	}
	for (cnt = 0;cnt< FillCubedel/8; cnt++)
	{
		LED_hor_plane_on(0);
		LED_hor_plane_on(1);
		LED_hor_plane_on(2);
	}
	for (cnt = 0;cnt< FillCubedel/2; cnt++)
	{
		LED_On(0,1);	LED_On(0,5);	LED_On(0,2);	LED_On(0,4);
		LED_On(1,1);	LED_On(1,5);	LED_On(1,2);	LED_On(1,4);
	}
	for (cnt = 0;cnt< FillCubedel*3; cnt++)
	{
		LED_On(0,1);
	}
	for (cnt = 0;cnt< FillCubedel/2; cnt++)
	{
		LED_On(0,1);	LED_On(0,5);	LED_On(0,2);	LED_On(0,4);
		LED_On(1,1);	LED_On(1,5);	LED_On(1,2);	LED_On(1,4);
	}
	for (cnt = 0;cnt< FillCubedel/8; cnt++)
	{
		LED_hor_plane_on(0);
		LED_hor_plane_on(1);
		LED_hor_plane_on(2);
	}
	for (cnt = 0;cnt< FillCubedel/2; cnt++)
	{
		LED_On(2,9);	LED_On(2,5);	LED_On(2,6);	LED_On(2,8);
		LED_On(1,9);	LED_On(1,5);	LED_On(1,6);	LED_On(1,8);
	}
}

void OneByOneLEDOn()
{
	int a[50],b[50];
	for (int k = 0; k < 30; k++)
	{
		a[k] = rand()%3;
		b[k] = rand()%10;
		
		for (int i = 0; i < k; i++)
		{
			LED_On(a[i],b[i]);
		}
	}	
}
//cnt = 0;cnt< ; cnt++

void RandomFall()
{
	int r = 1+rand()%9;
	for (int level = 2; level >=0; level--)
	{
		for (cnt = 0; cnt<fall_dell*10;cnt++)
		{
			LED_On(level,r);
		}
	}
	for (int level = 1; level <=2; level++)
	{
		for (cnt = 0; cnt<fall_dell*10;cnt++)
		{
			LED_On(level,r);
		}
	}
}

void Vret_Rect_loop()
{
	for (cnt = 0;cnt<del;cnt++)
	{
		LED_On(2,4);	LED_On(2,5);	LED_On(2,6);
		LED_On(1,4);	LED_On(1,5);	LED_On(1,6);
		LED_On(0,4);	LED_On(0,5);	LED_On(0,6);
	}
	for (cnt = 0;cnt<del;cnt++)
	{
		LED_On(2,3);	LED_On(2,5);	LED_On(2,7);
		LED_On(1,3);	LED_On(1,5);	LED_On(1,7);
		LED_On(0,3);	LED_On(0,5);	LED_On(0,7);
	}
	for (cnt = 0;cnt<del;cnt++)
	{
		LED_On(2,2);	LED_On(2,5);	LED_On(2,8);
		LED_On(1,2);	LED_On(1,5);	LED_On(1,8);
		LED_On(0,2);	LED_On(0,5);	LED_On(0,8);
	}
	for (cnt = 0;cnt<del;cnt++)
	{
		LED_On(2,1);	LED_On(2,5);	LED_On(2,9);
		LED_On(1,1);	LED_On(1,5);	LED_On(1,9);
		LED_On(0,1);	LED_On(0,5);	LED_On(0,9);
	}
}

void Plane_flip()
{
	for (int i = 0; i < 3; i++)
	{
		for (cnt = 0;cnt<del*2;cnt++)
		{
			LED_On(2,7);	LED_On(2,8);	LED_On(2,9);
			LED_On(1,7);	LED_On(1,8);	LED_On(1,9);
			LED_On(0,7);	LED_On(0,8);	LED_On(0,9);
		}
		for (cnt = 0;cnt<del/3;cnt++)
		{
			LED_On(2,7);	LED_On(2,8);	LED_On(2,5);	LED_On(2,6);
			LED_On(1,7);	LED_On(1,8);	LED_On(1,5);	LED_On(1,6);
			LED_On(0,7);	LED_On(0,8);	LED_On(0,5);	LED_On(0,6);
		}
		for (cnt = 0;cnt<del/3;cnt++)
		{
			LED_On(2,7);	LED_On(2,5);	LED_On(2,3);
			LED_On(1,7);	LED_On(1,5);	LED_On(1,3);
			LED_On(0,7);	LED_On(0,5);	LED_On(0,3);
		}
		for (cnt = 0;cnt<del/3;cnt++)
		{
			LED_On(2,7);	LED_On(2,4);	LED_On(2,5);	LED_On(2,2);
			LED_On(1,7);	LED_On(1,4);	LED_On(1,5);	LED_On(1,2);
			LED_On(0,7);	LED_On(0,4);	LED_On(0,5);	LED_On(0,2);
		}
		for (cnt = 0;cnt<del*2;cnt++)
		{
			LED_On(2,7);	LED_On(2,4);	LED_On(2,1);
			LED_On(1,7);	LED_On(1,4);	LED_On(1,1);
			LED_On(0,7);	LED_On(0,4);	LED_On(0,1);
		}
		for (cnt = 0;cnt<del/3;cnt++)
		{
			LED_On(2,1);	LED_On(2,4);	LED_On(2,5);	LED_On(2,8);
			LED_On(1,1);	LED_On(1,4);	LED_On(1,5);	LED_On(1,8);
			LED_On(0,1);	LED_On(0,4);	LED_On(0,5);	LED_On(0,8);
		}
		for (cnt = 0;cnt<del/3;cnt++)
		{
			LED_On(2,1);	LED_On(2,5);	LED_On(2,9);
			LED_On(1,1);	LED_On(1,5);	LED_On(1,9);
			LED_On(0,1);	LED_On(0,5);	LED_On(0,9);
		}
		for (cnt = 0;cnt<del/3;cnt++)
		{
			LED_On(2,1);	LED_On(2,2);	LED_On(2,5);	LED_On(2,6);
			LED_On(1,1);	LED_On(1,2);	LED_On(1,5);	LED_On(1,6);
			LED_On(0,1);	LED_On(0,2);	LED_On(0,5);	LED_On(0,6);
		}
		for (cnt = 0;cnt<del*2;cnt++)
		{
			LED_On(2,1);	LED_On(2,2);	LED_On(2,3);
			LED_On(1,1);	LED_On(1,2);	LED_On(1,3);
			LED_On(0,1);	LED_On(0,2);	LED_On(0,3);
		}
		for (cnt = 0;cnt<del/3;cnt++)
		{
			LED_On(2,2);	LED_On(2,4);	LED_On(2,5);	LED_On(2,3);
			LED_On(1,2);	LED_On(1,4);	LED_On(1,5);	LED_On(1,3);
			LED_On(0,2);	LED_On(0,4);	LED_On(0,5);	LED_On(0,3);
		}
		for (cnt = 0;cnt<del/3;cnt++)
		{
			LED_On(2,7);	LED_On(2,5);	LED_On(2,3);
			LED_On(1,7);	LED_On(1,5);	LED_On(1,3);
			LED_On(0,7);	LED_On(0,5);	LED_On(0,3);
		}
		for (cnt = 0;cnt<del/3;cnt++)
		{
			LED_On(2,3);	LED_On(2,8);	LED_On(2,5);	LED_On(2,6);
			LED_On(1,3);	LED_On(1,8);	LED_On(1,5);	LED_On(1,6);
			LED_On(0,3);	LED_On(0,8);	LED_On(0,5);	LED_On(0,6);
		}
		for (cnt = 0;cnt<del*2;cnt++)
		{
			LED_On(2,3);	LED_On(2,6);	LED_On(2,9);
			LED_On(1,3);	LED_On(1,6);	LED_On(1,9);
			LED_On(0,3);	LED_On(0,6);	LED_On(0,9);
		}
		for (cnt = 0;cnt<del/3;cnt++)
		{
			LED_On(2,2);	LED_On(2,9);	LED_On(2,5);	LED_On(2,6);
			LED_On(1,2);	LED_On(1,9);	LED_On(1,5);	LED_On(1,6);
			LED_On(0,2);	LED_On(0,9);	LED_On(0,5);	LED_On(0,6);
		}
		for (cnt = 0;cnt<del/3;cnt++)
		{
			LED_On(2,1);	LED_On(2,5);	LED_On(2,9);
			LED_On(1,1);	LED_On(1,5);	LED_On(1,9);
			LED_On(0,1);	LED_On(0,5);	LED_On(0,9);
		}
		for (cnt = 0;cnt<del/3;cnt++)
		{
			LED_On(2,9);	LED_On(2,8);	LED_On(2,5);	LED_On(2,4);
			LED_On(1,9);	LED_On(1,8);	LED_On(1,5);	LED_On(1,4);
			LED_On(0,9);	LED_On(0,8);	LED_On(0,5);	LED_On(0,4);
		}
	}
	for (cnt = 0;cnt<del*2;cnt++)
	{
		LED_On(2,7);	LED_On(2,8);	LED_On(2,9);
		LED_On(1,7);	LED_On(1,8);	LED_On(1,9);
		LED_On(0,7);	LED_On(0,8);	LED_On(0,9);
	}
}

void InOut()
{
	for (cnt = 0;cnt<InOutdel*20;cnt++)
	{
		LED_On(1,5);
	}
	for (cnt = 0;cnt<InOutdel;cnt++)
	{
		LED_hor_plane_on(0);
		LED_hor_plane_on(1);
		LED_hor_plane_on(2);
	}
}

/*

1	2	3
4	5	6
7	8	9

*/
