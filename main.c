#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define NUM 5

struct item {//商品
	char brand[20];//商品名
	char id[10];//商品编号
	float come_price;//进价
	float out_price;//售价
	int num;//数量
};

struct item_shop {//购物车类型
	struct item wanted;//商品
	int amount;//购物数量
	struct item_shop* next;
};

struct item goods[NUM];
struct item_shop* car;

int menu()//开始菜单
{
	int select;
	printf("请选择操作（数字1——5）\n");
	printf("1.建立库存信息\n");
	printf("2.显示库存信息\n");
	printf("3.购物车\n");
	printf("4.结算\n");
	printf("5.退出\n");
	printf("请选择对应数字\n");//初始模板
	printf("您的选择是:");

	while (1)
	{
		scanf("%d", &select);//读取选择的数字
		if (select < 1 || select>5)
		{
			printf("输入了错误的数字，请重新输入：\n");
		}
		else
		{
			break;
		}
	}
		return select;
}

//建立库存函数
void establish() {
	FILE* fp;//创建指向文件的指针
	printf("请依次输入货物信息：\n");
	printf("——————————————————————————————————————\n");

	//初始化商品信息存入goods
	for (int i = 0; i < NUM; i++)
	{
		printf("商品名:");
		fflush(stdin);
		scanf("%s", &goods[i].brand);

		printf("货号:");
		fflush(stdin);
		scanf("%s", &goods[i].id);

		printf("进价:");
		fflush(stdin);
		scanf("%f", &goods[i].come_price);

		printf("售价:");
		fflush(stdin);
		scanf("%f", &goods[i].out_price);

		printf("数量:");
		fflush(stdin);
		scanf("%d", &goods[i].num);
		printf("————————————————————————————\n");
	}

	//判断文件是否能创建
	if ((fp = fopen("goods", "w")) == NULL)
	{
		printf("创建文件失败!\n");
		return 0;
	}
	//把goods数组的内容写入文件
	fwrite(&goods, sizeof(struct item), NUM, fp);
	//关闭文件
	fclose(fp);

}

//显示所有商品信息
void dis_all()
{
	system("cls");//清除前面的内容
	FILE* fp;
	fp = fopen("goods", "r");
	for (int i = 0; (fread(goods + i, sizeof(struct item), 1, fp)) != 0; i++)
	{
		printf("——————————————————————————————————————\n");
		printf("编号\t商品\t价格\t数量\n");
		printf("%s%8s%8.2f%7d\n", goods[i].id, goods[i].brand, goods[i].out_price, goods[i].num);
	}
	fclose(fp);
}

//购物车：菜单
int car_menu()
{
	int select;
	printf("请选择操作:\n");
	printf("————————————————————————————————\n");
	printf("1.添加商品\n");
	printf("2.显示当前购物列表\n");
	printf("3.退出\n");
	printf("请选择对应数字\n");
	printf("————————————————————————————————\n");
	printf("您的选择是:");

	while (1)
	{
		scanf("%d", &select);//读取选择的数字
		if (select < 1 || select>3)
		{
			printf("输入了错误的数字，请重新输入：\n");
		}
		else
		{
			break;
		}
	}
	return select;
}

//显示购物列表
void display()
{
	struct item_shop* p = car;
	if (p == NULL)
	{
		printf("购物车为空！\n");
		return;
	}
	while (p != NULL)
	{
		printf("——————————————————————————————————————\n");
		printf("编号\t商品\t价格\t数量\n");
		printf("%s%8s%8.2f%7d\n", p->wanted.id,p->wanted.brand,p->wanted.out_price,p->amount);
		p = p->next;//跳转下一个节点
	}
}

//购物车：添加商品
void shop_add()
{
	FILE* fp;
	char str[20];//获取物品名字,编号
	int n;//获取物品数量
	char choice;
	char choice2;
	struct item_shop* p1,*p;
	int i;
	do
	{
		printf("输入买入的物品名称或编号:\n");
		fflush(stdin);
		scanf("%s", str);
		if ((fp = fopen("goods", "r")) == NULL)
		{
			printf("系统加载失败！！\n");
			continue;
		}
		for (i = 0; fread(goods + i, sizeof(struct item), 1, fp) != 0; i++)
		{
			//判断：编号，名称不能错，并且商品的库存数量不能等于0.
			if ((strcmp(goods[i].brand, str) == 0 || strcmp(goods[i].id, str) == 0) && goods[i].num != 0)
			{
				printf("已经找到所需物品\n");
				printf("————————————————————————————————\n");
				printf("编号\t商品\t价格\t数量\n");
				printf("%s%8s%8.2f%7d\n", goods[i].id, goods[i].brand, goods[i].out_price, goods[i].num);
				printf("请输入所需要的数量:");
				scanf("%d", &n);
				fflush(stdin);
				if (n > goods[i].num)
				{
					printf("当前所需数量大于库存数\n");
					break;
				}
				printf("是否购买(Y/N):");
				scanf(" %c", &choice);
				if (choice == 'Y' || choice == 'y')
				{
					p1 = (struct item_shop*)malloc(sizeof(struct item_shop));
					if(p1 == NULL)
					{
						printf("内存申请失败！\n");
						exit(1);
					}
					p1->amount = n;//存入购物数量
					p1->wanted = goods[i];
					p1->next = NULL;
					p = car;
					if (car == NULL)
					{
						car = p1;
					}
					else
					{
						p = car;
						while (p->next != NULL)
						{
							p = p->next;
						}
						p->next = p1;
					}
					printf("成功加入购物车！\n");
				}	
					
				break;
			}
		}
		if (i == NUM)
		{
			printf("未找到所需要物品\n");
		}
		fclose(fp);
		printf("是否需要继续购物?(Y/N):\n");
		scanf(" %c", &choice2);
	} while (choice2 == 'Y' || choice2 == 'y');
}

//购物车
void shop_car()
{
	while (1)
	{
		switch (car_menu())
		{
		case 1:
		{
			//printf("1.添加商品\n");
			shop_add();
		}
			break;
		case 2:
		{
			//printf("2.显示当前购物列表\n");
			display();
		}
			break;
		case 3:
		{

			return;
		}
			break;
		}
	}

}

//结算
void count_up()
{
	float total = 0, pay;
	struct item_shop* p;
	FILE* fp;
	if (car == NULL)
	{
		printf("当前购物车为空\n");
		return;
	}
	printf("购物清单:\n");
	display();
	if ((fp = fopen("goods", "r"))== NULL)
	{
		printf("系统繁忙，请稍后重试\n");
		return;
	}
	for (int i = 0; (fread(goods + i, sizeof(struct item), 1, fp))!=0;i++)
	{
		fclose(fp);
		p = car;

		while(p!=NULL)
		{
			total += p->wanted.out_price * p->amount;
			for (int i= 0; strcmp(goods[i].id, p->wanted.id) != 0; i++)
			{
				goods[i].num -= p->amount;
				break;
			}
				p = p->next;
		}
		printf("总计：%7.2f", total);
		printf("\n输入支付金额：");
		scanf("%f", &pay);
		printf("实际支付金额：%7.2f\t\t找零:%7.2f\n",pay,pay-total);
		printf("——————————————————————————————————————\n");
		if ((fp = fopen("goods", "w")) == NULL)
		{
			printf("支付失败，请稍后重试\n");
			return;
		}
		fwrite(goods, sizeof(struct item), NUM, fp);
		fclose(fp);
	}
}


int main()
{
	printf("**************************************\n");
	printf("\t欢迎进入超市管理系统\n");
	printf("**************************************\n");
	
	while(1){
		switch (menu())
		{
		case 1://printf("1.建立库存信息\n");
		{
			establish();
		} break;	
		case 2://printf("2.显示库存信息\n");
		{
			dis_all();
		}
			break;
		case 3://printf("3.购物车\n");
		{
			shop_car();
		}
			break;
			case 4://printf("4.结算\n");
			{
				count_up();
			}
				break;
			case 5://printf("5.退出\n");
			{
				system("cls");
				printf("**************************************\n");
				printf("感谢您的使用，欢迎下次光临，再见！\n");
				printf("**************************************\n");
				exit(0);
			}
				break;
		}
	}

	return 0;
}