#include "JohnB.h"




//////////////////////////////////////////// ENTITIES METHODS
basic_entity::basic_entity(WINDOW *b_win, int b_y, int b_x, int b_chr, st_e_p *s_buf, int *b_ccp)
        : win{b_win}, y{b_y}, x{b_x}, chr{b_chr}, st_stack{s_buf}, _ccp{b_ccp}{}
basic_entity::~basic_entity(){};
int basic_entity::retX(){return x;}
int basic_entity::retY(){return y;}
int basic_entity::retLife(){return life;}
int basic_entity::retDirection(){return direction;}

void basic_entity::show_e(){ mvwaddch(win, y, x, direction); }

void basic_entity::changeDirection(int n_d){ direction = n_d; }

void basic_entity::incLife(){ life ++; }
void basic_entity::decLife(){ life --; }

void basic_entity::inc_ccp(){(*_ccp)++;}
void basic_entity::dec_ccp(){(*_ccp)--;}
int* basic_entity::ret_ccp_ptr(){return _ccp;}
int basic_entity::ret_ccp_val(){return *_ccp;}


void basic_entity::setX(int x_b){x = x_b;};
void basic_entity::setY(int y_b){y = y_b;};
void basic_entity::incX(){ x++; }
void basic_entity::incY(){ y++; }
void basic_entity::decX(){ x--; }
void basic_entity::decY(){ y--; }

int basic_entity::new_x_based_on_direction()
{
	int b_x = retX();
	if(direction == LEFT)
		b_x --;
	if(direction == RIGHT)	
		b_x ++;

	return b_x;
		
}
int basic_entity::new_y_based_on_direction()
{
	int b_y = retY();
	if(direction == UP)
		b_y --;
	if(direction == DOWN)	
		b_y ++;

	return b_y;
	
}

bool basic_entity::collision(st_e_p ents_stack)
{ 
	int b_y = new_y_based_on_direction();
	int b_x = new_x_based_on_direction();
	bool flag = true;
	
	while(ents_stack != NULL && flag){
		if ( (ents_stack -> x == b_x+ret_ccp_val()) && (ents_stack -> y == b_y) ) flag = false;
		ents_stack = ents_stack -> next;
	}
		
	return flag; 
} 

void basic_entity::win_mv_u(){

	direction = UP;
	if( y > 2 && collision(*st_stack))
		decY();
}

void basic_entity::win_mv_d(){  

	direction = DOWN;
	if(y < ROWS-2 && collision(*st_stack))
		incY();
}

//////////////////////////////////////////// PLAYER CLASS METHODS
//constructor
//kb_mv
//win_mv_l
//win_mv_r
//
///////////////////////////////////////////
player::player(WINDOW *b_win, int b_y, int b_x, int b_chr, st_e_p *s_b, int *b_ccp)
	: basic_entity(b_win, b_y, b_x, b_chr, s_b, b_ccp) {} 

player::~player(){};

int player::retWood() { return wood; }
void player::incWood(){ wood++; }
void player::decWood(){ wood--; }

void player::kb_mv(int choice){
	if(choice >= 'A' && choice <= 'z')
		switch(choice){
			case 'a': case 'A':
				win_mv_l();
			break;
	
			case 'd': case 'D':
				win_mv_r();
			break;

			case 's': case 'S':
				win_mv_d();
			break;

			case 'w': case 'W':
				win_mv_u();
			break;
			
			case 'r': case 'R':
				take();
			break;
			
			case 'e': case 'E':
				place();
			break;
		}
}
void player::win_mv_l(){
	
	direction = LEFT;
	if(collision(*st_stack)){
		decX();
		if( retX() == BEG_OFFSET ){
			setX(BEG_OFFSET + 1);
			dec_ccp();
		}
	}
}

void player::win_mv_r(){

	direction = RIGHT;
	if(collision((*st_stack))){
		incX();
		if(retX() == END_OFFSET ){		//IDEA: set il pg raggiunge la fine dell'offset, viene aumentato _cpp.
			setX(END_OFFSET - 1);		//non ricordo perchè ho chiamato _ccp in questo modo boh	
			inc_ccp();
			if(ret_ccp_val() > max_ccp_reached){
				rand_gen_tree(st_stack, ret_ccp_val());
				max_ccp_reached++;
			}
		}
	}
}

st_e_p player::find_s_e()
{
	
	int b_y = new_y_based_on_direction();
	int b_x = new_x_based_on_direction();
	st_e_p tmp = (*st_stack);
	
	while(tmp!=NULL){
		if((tmp -> x == b_x+ret_ccp_val())&& tmp -> y == b_y)
			return tmp;
		tmp = tmp->next;
	}
	return NULL;
}
void player::take()
{
	st_e_p buff = (*st_stack);
	st_e_p tmp = find_s_e();
	if(tmp!=NULL && buff!=NULL){
		if(tmp == buff){
			(*st_stack) = (*st_stack)->next;
			delete buff;
		}
		else{
			while(buff != NULL){
				if(buff -> next == tmp)
					buff -> next = buff -> next -> next;
				 buff = buff -> next;
			}
		}
		incWood();
	}
}

void player::place()
{		
	
	int b_y = new_y_based_on_direction();
	int b_x = new_x_based_on_direction();
	bool flag = true;
	if(retWood() > 0){
		st_e_p tmp = new static_ent;	
		tmp -> x = b_x+ret_ccp_val();
		tmp -> y = b_y;
		tmp -> chr_code = '|';
		tmp -> next = NULL;
		st_e_p buff = (*st_stack);
		while(buff != NULL && flag){
			if( buff->next != NULL ){
				if( ( (buff->next) -> x <= (tmp -> x)  ) ){
					tmp -> next = buff -> next;
					buff -> next = tmp;
					flag = false;
				}
			}
			else {buff->next = tmp; flag = false;};

			buff = buff -> next;	
		}
		
		decWood();	
	}
}
//////////////////////////////////////////// LIBRARY FUNCTIONS
void rand_gen_tree( st_e_p *tree, int _ccp )
{
        srand(time(0));
        int n = (rand() % 8 + 1);
	st_e_p tmp = new static_ent;
	tmp -> chr_code = TREE;
        tmp -> x = COLS + _ccp;			
        tmp -> y = n;
        tmp -> next = *tree;
        (*tree) = tmp;

}



void show_s_ents(WINDOW * win, st_e_p buff_es, int _ccp)
{
	int i = COLS*ROWS;
	while(buff_es != NULL && i>0){
		mvwaddch(win, buff_es->y, (buff_es->x)-_ccp, buff_es -> chr_code);
		buff_es = buff_es -> next;
		i--;
	}
}

void frame_refresh(WINDOW *win, player *p, st_e_p buff_es, int _ccp)
{
        usleep(tm);
        werase(win);
        p->show_e();
	show_s_ents(win, buff_es, _ccp);
        wborder(win, '*', '*', '*', '*', '*', '*', '*', '*' );
        wrefresh(win);
}

















