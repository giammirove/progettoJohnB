#ifndef JohnB
#define JohnB

#include <iostream>
#include <ncurses.h>
#include <unistd.h>
#include <time.h>

#define COLS 60
#define ROWS 10
#define LEFT '<'
#define RIGHT '>'
#define DOWN 'v'
#define UP '^'
#define END_OFFSET 45
#define BEG_OFFSET 2

#define TREE 'T'
const int tm = 60000;

struct static_ent{
	int x;
	int y;
	int chr_code;
	int *_ccp;
	static_ent *next = NULL;
};

typedef static_ent *st_e_p;


void rand_gen_tree(st_e_p *, int);
void show_ents(WINDOW *, st_e_p, int);


class basic_entity{
	public:
		//	Window,      Y    X     CHAR  
		basic_entity(WINDOW *, int, int, int, st_e_p *, int *);
		~basic_entity();
		int retX(); 		//return X
		int retY();		//return Y
		
		void setX(int);
		void setY(int);

		int retLife();		//return Life
		void incLife();
		void decLife();

		void inc_ccp();
		void dec_ccp();
		int *ret_ccp_ptr();
		int ret_ccp_val();

		int retDirection();
		void changeDirection(int); 

		void win_mv_u();     //Muoversi verso Nord
		void win_mv_d();     //Muoversi verso Sud
		
		int new_x_based_on_direction();
		int new_y_based_on_direction();

		bool collision(st_e_p);
		void show_e();		//Mostra l'entità

	protected:		

		int y, x;
		int life = 5;
		int chr;
		int ammo = 10;
		
		int max_ccp_reached = 0;	
		int *_ccp;
		int direction = LEFT;

		WINDOW *win;

		void incX();		//increase and decrease	
		void incY();
		void decX();
		void decY();
		st_e_p *st_stack;
		
};

class player : public basic_entity{
	public:	
		player(WINDOW *, int, int, int, st_e_p *, int *);
		~player();
		void kb_mv(int); //pressed key
	
		void win_mv_l();
		void win_mv_r();
		
		void craft_door();
		void craft_ammo();
		
		void place();
		void take();
		
		int retWood();
		void incWood();
		void decWood();
	private:
		st_e_p find_s_e();
		int wood = 0;
		int dors = 0;
};

class monster : public basic_entity{
	public:
		monster();
		void stalk(player *);
		void win_mv_l();
		void win_mv_r();
};

//////////////////////////////////////FUNCTIONS



void frame_refresh(WINDOW *, player *, st_e_p, int);

#else
#endif
