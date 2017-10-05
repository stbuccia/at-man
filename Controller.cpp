#include "Controller.hpp"

Controller::Controller(GestoreLivelli gl, Giocatore* player){
	this->gestore = gl;
	this->p = player;
	this->d = new Drawer();
	this->stanza = gestore.getInizio()->getStanza(0);
	p->setPosX(stanza.getLibero());
	p->setPosY(stanza.getDimensione()-2);
	ended=false;
}

void Controller::vaiSu(){
	d->liberaPosizione(&stanza, p->getPosY(), p->getPosX());
	p->setPosY(p->getPosY()-1);

}
void Controller::vaiGiu(){
	d->liberaPosizione(&stanza, p->getPosY(), p->getPosX());
	p->setPosY(p->getPosY()+1);
}
void Controller::vaiDx(){
	d->liberaPosizione(&stanza, p->getPosY(), p->getPosX());
	p->setPosX(p->getPosX()+1);
}
void Controller::vaiSx(){
	d->liberaPosizione(&stanza, p->getPosY(), p->getPosX());
	p->setPosX(p->getPosX()-1);
}


void Controller::cambiaStanza(int direzione){
	Livello* l=gestore.getLevelById(gestore.getLivello());
	int coll[4];
	int oldColl[4], inizio=0;
	int oldId=stanza.getId();

	for (int i=0; i<4; i++){
		coll[i]=stanza.getColl(i);
		oldColl[i]=coll[i];
	}

	if (coll[direzione]==-2){
		//CASO CAMBIO LIVELLO
		oldId=-2;
		if ((stanza.getId()+1)==gestore.getLivello()){
			gestore.creaLivello(gestore.getLivello() + 1);
			l=gestore.getLevelById(gestore.getLivello());
			stanza=l->getStanza(0);
		}
		else{
			gestore.passaLivPrec();
			l=gestore.getLevelById(gestore.getLivello());
			stanza=l->getStanza(l->getNStanze()-1);
		}
	}
	else{
		//CASO STESSO LIVELLO
		if (direzione==0) stanza=l->vaiNord(stanza.getId());
		else if (direzione==1) stanza=l->vaiSud(stanza.getId());
		else if (direzione==2) stanza=l->vaiOvest(stanza.getId());
		else stanza=l->vaiEst(stanza.getId());
	}
	int cont=0;
	for (int i=0; i<direzione; i++){
		if(oldColl[i]==stanza.getId())
			cont++;
	}
	//STABILISCE LA MIA NUOVA POSIZIONE NELLA STANZA
	for (int i=0; i<4; i++){
		coll[i]=stanza.getColl(i);
		if (coll[i]==oldId){
			if (cont==0)
				inizio=i;
			cont--;

		}
	}
	if (inizio==0){
		p->setPosX(stanza.getPorta(inizio));
		p->setPosY(1);
	}
	else if (inizio==1) {
		p->setPosX(stanza.getPorta(inizio));
		p->setPosY(stanza.getDimensione()-2);
	}
	else if (inizio==2) {
		p->setPosX(1);
		p->setPosY(stanza.getPorta(inizio));
	}
	else {
		p->setPosX(stanza.getDimensione()-2);
		p->setPosY(stanza.getPorta(inizio));
	}

	//DISEGNO
	d->disegnaStanza(&stanza);
	l->visitStanza(stanza.getId());
	d->disegnaLiv(l, gestore.getLivello());
	d->posizionaGiocatore(&stanza, p);
}

//Giulia's metodo
void Controller::gestisciInput(char c){
    keypad(stdscr, true);
    switch(c){

        //MOVIMENTO IN ALTO
        case ((char)KEY_UP):
            if (controllaMovimento(p->getPosX(), p->getPosY()-1)==true)
            {
                if(p->actMuovi()){
                    this->vaiSu();
                    d->posizionaGiocatore(&stanza, p);
                    d->disegnaStat(p);
                }
            }
        break;

        //MOVIMENTO IN BASSO
        case ((char)KEY_DOWN):
            if (controllaMovimento(p->getPosX(), p->getPosY()+1)==true)
            {
                if(p->actMuovi()){
                    this->vaiGiu();
                    d->posizionaGiocatore(&stanza, p);
                    d->disegnaStat(p);
                }
            }
        break;

        //MOVIMENTO A DESTRA
        case ((char)KEY_RIGHT):
            if (controllaMovimento(p->getPosX()+1, p->getPosY())==true)
            {
                if(p->actMuovi()){
                    this->vaiDx();
                    d->posizionaGiocatore(&stanza, p);
                    d->disegnaStat(p);
                }
            }
        break;

        //MOVIMENTO A SINISTRA
        case ((char)KEY_LEFT):
            if (controllaMovimento(p->getPosX()-1, p->getPosY())==true)
            {
                if(p->actMuovi()){
                    this->vaiSx();
                    d->posizionaGiocatore(&stanza, p);
                    d->disegnaStat(p);

                }
            }
        break;
        //APRIRE PORTA
        case ((char)('a')):
            int dir;
            if(isVicinoPorta(dir)){
                this->cambiaStanza(dir);
                char msg[100];
                sprintf (msg, "Stanza cambiata");
                d->disegnaMess(msg);
            }
        break;
        case((char)('x')):
            endwin();
            ended=true;
        break;
        //COMPRARE DA VENDIORE  (B)
        case((char)('b')):
        break;
        //ATTACCARE MOSTRO      (K)
        case((char)('k')):
        break;
        //APRIRE BAULE          (O)
        case((char)('o')):
        break;
        //RACCOGLIERE OGGETTI LASCIATI DAL MOSTRO

        //USA POZIONE           (P)
        case((char)('p')):
        break;
        //CAMBIA ARMA           (C)
        case((char)('c')):
        break;
        //SCARTA ARMA           (S)
        case((char)('s')):
        break;

        //HELP                  (H)
        case((char)('h')):
        break;

        default:
            char msg[100];
            sprintf (msg, "Clicca un pulsante riconosciuto              ");
            d->disegnaMess(msg);
    }

}

bool Controller::isVicinoPorta(int &dir){
		int x=p->getPosX();
		int y=p->getPosY();
		dir=-1;
		if (x==stanza.getPorta(0) && y==1) dir=0;
		if (x==stanza.getPorta(1) && y==stanza.getDimensione()-2) dir=1;
		if (x==1 && y==stanza.getPorta(2)) dir=2;
		if (x==stanza.getDimensione()-2 && y==stanza.getPorta(3)) dir=3;

		return (dir!=-1);

}

bool Controller::controllaMovimento(int posX, int posY){
    bool valido=false;
    int m [MAXDIM][MAXDIM];
    stanza.getMatrice(m);

    if(m[posY][posX]==-3|| m[posY][posX]==-2|| m[posY][posX]==-1|| m[posY][posX]==4 ) {
        if(m[posY][posX]==4){
            //CAMBIO STANZA


            //PASSAGGIO LIVELLO
        }
        else{
            char msg[100];
            sprintf (msg, "Spostamento effettuato                   ");
            d->disegnaMess(msg);
            valido=true;
        }


    }
    else{
        char msg[100];
        sprintf (msg, "Spostamento negato");
        d->disegnaMess(msg);
    }
    return valido;
}
bool Controller::hasGameEnded()
{
    return ended;
}
void Controller::init()
{
    initscr();
	raw();
	noecho();
	refresh();
    d->disegna(p, gestore.getInizio(), &stanza);
}
void Controller::gioca(){

        char c=' ';
        c=tolower(getch());
        gestisciInput(c);
        usleep(30000);
}


void Controller::printDebugMsg(const char* s)
{
    char msg[100];
    sprintf(msg, s);
    d->disegnaMess(msg);
}


Stanza* Controller::getCurrentRoom()
{
    return &stanza;
}