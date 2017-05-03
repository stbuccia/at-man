#include "GeneratoreLivelli.hpp"
GeneratoreLivelli::GeneratoreLivelli(){
	srand(time(0));
}



void GeneratoreLivelli:: inizializzaVettColl(Livello* l){
	int i;
	Collegamento* c;
	for(i=0; i<l->getNStanze(); i++){
		c=new Collegamento(i);
		l->setCollegamento(i,c);
	}

}


void GeneratoreLivelli::riempiVettStanze(Livello* l){
	int i;
	Stanza* s;
	for(i=0; i<l->getNStanze()+2; i++){

		if(i==l->getNStanze()||i==l->getNStanze()+1){
			/** NOTA: PER COMODITA' I COLLEGAMENTI AI LIVELLI PRECEDENTI E SUCCESSIVI SONO STATI MESSI A -1 CIOE' LE STANZE DEI LIVELLI PREC E SUCC HANNO COME NUMERO INTERNO -1
			*/
			s= new Stanza(-2);
			l->setStanza(i,s);
				//cout<< i << ":"<<(l->getStanza(i)).getS()<<endl;
		}
		else{
			s= new Stanza(i);
			l->setStanza(i,s);
				//cout<<i<<":"<<(l->getStanza(i)).getS()<<endl;
		}
	}
}

void GeneratoreLivelli::controllaAvail(bool avail[4],Livello* l){
	int a[4];
	int neg;
	//scorro le stanze
	for(int i=0; i<l->getNStanze();i++){
		neg=0;
		l->getAdiacenze(i,a);
		//conto quante direzioni sono occupate
		for(int j=0;j<4;j++){
			if(a[j]!=-1){
				neg++;
			}
		}

		//se le direzioni occupate sono 4 => setto a false
		if(neg==4){
			avail[i]=false;
		}
		//se le direzioni occupate sono 3 e si tratta o della prima o dell'ultima stanza => setto a false
		else if(neg==3&&(i==0||i==l->getNStanze()-1)){
				avail[i]=false;
		}
		//altrimenti setto a true;
		else{
				avail[i]=true;
		}
	}
}

void GeneratoreLivelli::controllaAvail2(bool avail[4],Livello* l){
	int a[4];
	int neg;

	//scorro le stanze
	for(int i=0; i<l->getNStanze();i++){
		//se la stanza non è già settata a false faccio il controllo
		 if(avail[i]!=false){
		 	cout<<"entrato "<<i;
			neg=0;
			l->getAdiacenze(i,a);


			//conto quante direzioni sono occupate
			for(int j=0;j<4;j++){
				cout<<a[j]<<" ";
				if(a[j]!=-1){
					neg++;
				}
			}cout<<endl;

			//se le direzioni occupate sono 4 => setto a false
			if(neg==4){
				avail[i]=false;
				cout<<"cancello "<<i<<endl;

			}
			//se le direzioni occupate sono 3 e si tratta o della prima o dell'ultima stanza => setto a false
			else if(neg==3&&(i==0||i==l->getNStanze()-1)){
				avail[i]=false; cout<<"cancello "<<i<<endl;
			}
		}
	}
}

void GeneratoreLivelli::riempiVettCollegamenti(Livello* l, int maxLink){

	int p = 0;
	int a = 0;
	int stanze = l->getNStanze();
	bool avail[stanze];

	/**PRIMA PARTE*/
	// Preparo vettore delle disponibilità
	for (int i = 0; i < stanze; ++i){ avail[i] = true; }
		avail[p] = false;

	cout<<"RIEMPI"<<endl;

	// Primo giro dei collegamenti
	for (int i = 0; i < stanze - 1; ++i) {
		for (int i = 0; i < stanze; ++i)
	{
				printf("%d, ", avail[i] );

	}
		// Scelgo la stanza
		do {
			a = rand() % stanze;
			//printf("sto scegliendo la stanza di arrivo\n");
		} while (avail[a] == false);

		// Collego le stanze
		link(p, a, l);
		//printf("ho collegato la stanza\n");
		avail[a] = false;
		controllaAvail2(avail,l);
		p = a;
	}

	// Abbiamo collegato tra loro tutte le stanze. Si
	// rigenera il vettore delle disponibilità e poi...

	/**SECONDA PARTE*/
	//controlla adiacenze delle stanze e toglie le stanze piene per evitare di collegarle
	controllaAvail(avail,l);

	printf("_____________________________________________________________\n");

	// Si collegano un'altra volta le stanze per garantire
	// dei doppi collegamenti

	for (int i = 0; i < maxLink - stanze + 1; ++i) {
		p = rand() % stanze; //inizializziamo p
		a = rand() % stanze; //inizializziamo a

		// Scelgo la stanza a cui arrivare in modo che sia accessibile
		while (avail[a] == false){
			a = rand() % stanze;
		};

		// Scelgo la stanza da cui partire in modo che sia accessibile
		while (avail[p] == false){
			p = rand() % stanze;
		};

		// se le due stanze sono uguali cambio la stanza di partenza
		while (p==a){
			p = rand() % stanze;
		};

		// Collego le stanze
		link(p, a, l);

		//setto a false la stanza da cui sono arrivata per toglierla dalle stanze accessibili
		avail[a]=false;

		//controlla adiacenze delle stanze e toglie le stanze piene per evitare di collegarle e in più
		controllaAvail2(avail,l);
	}
}


void GeneratoreLivelli::link(int da, int a, Livello* l){

	   /**
			NOTA: IL CONTROLLO SU STANZA DI PARTENZA E STANZA FINALE VA BENE BISOGNA AGGIUNGERE IL FATTO CHE SE
			COLLEGAMENTO NELLA DIREZIONE SCELTA E' PARI A -1 POSSO COLLEGARE ALTRIMENTI DEVO SCEGLIERE SCEGLIERE
			UN'ALTRA DIREZIONE CON IL RANDOM
	   */
		int stanze = l -> getNStanze() - 1;

		// direzione di arrivo da -> a
		int dA = rand() % 4;

		// direzione di partenza a -> da
		int dP = rand() % 4;


		int dirDa[4];
		int dirA[4];

		// Mi prendo le adiacenze della stanza da e a per i controlli
		l->getAdiacenze(da, dirDa);
		l->getAdiacenze(a, dirA);

		/** controlliamo nessuna direzione proibita nel collegamento da/a la prima e l'ultima stanza*/

		// se in da c'è l'ultima stanza e in dA c'è la direzione NORD (0) allora viene ricalcolato il random
		if(da == stanze&& dA==0){
			while(dA==0){
			   dA = rand() % 4;
			}
		}
		//se in a c'è l'ultima stanza e in dP c'è la direzione NORD (0) allora viene ricalcolato il random
		if(a == stanze&& dP==0){
			while(dP==0){
			   dP = rand() % 4;
			}
		}
		//se in da c'è la prima stanza e in dA c'è la direzione SUD (1) allora viene ricalcolato il random
		if(da == 0&& dA==1){
			while(dA==1){
			   dA = rand() % 4;
			}
		}

		//se in a c'è la prima stanza e in dP c'è la direzione SUD (1)allora viene ricalcolato il random
		if(a == 0&& dP==1){
			while(dP==1){
			   dP = rand() % 4;
			}
		}

	/** controlliamo che la direzione scelta per le due stanze sia libera (cioè settata a -1) */

	//se la direzione è occupata dA => faccio controllo che ne restituisce una giusta
	if(dirDa[dA]!=-1){
		dA=controlloLink(da,dA,dirDa,stanze);
		printf("Link controllato (dA) \n");
	}

	 //se la direzione è occupata dP => faccio controllo che ne restituisce una giusta
	if(dirA[dP]!=-1){
		dP=controlloLink(a,dP,dirA,stanze);
		//printf("Link controllato (dP) \n");
	}

	//adesso che sono sicura che le direzioni siano libere collego le due stanze
	printf("\t%d:%d -> %d:%d\n",da, dA , a, dP );
	l->collegaStanza(da,a,dA);
	l->collegaStanza(a,da,dP);
	printf("Stanze collegate \n");

}


int GeneratoreLivelli::controlloLink(int s,int direzione,int dir[4], int stanze){
	//printf("Dentro ControlloLink\n");
	//printf(" STANZA: %d", s);
	do{
		//cambio la direzione
		direzione = rand() % 4;

		//caso particolare dell'ultima stanza
		if(s == stanze&& direzione==0){
		   while(direzione==0){
		   		direzione = rand() % 4;
		   		//printf("Riscelta su ultima stanza \n");
			}
		}

		//caso particolare della prima stanza
		else if(s == 0&& direzione==1){
			while(direzione==1){
			   direzione = rand() % 4;
			   //printf("Riscelta su prima stanza \n");
			} ;
		}
		//printf("Di nuovo su");
	}while(dir[direzione]!=-1);//fino a quando la direzione non è libera
	
	//printf("\t DIREZIONE: %d, stanza: %d", direzione, dir[direzione]);

	return direzione;

}


void GeneratoreLivelli::stampaCollegamenti(Livello* l){
	int a[4]={0,0,0,0};
	cout<<endl;
	for (int i=0; i<l->getNStanze();i++){
		l->getAdiacenze(i,a);
		cout<<i<<":";
		for (int j=0; j<4; j++){
			cout<<"\t"<<a[j]<<" ";
		}
		cout<<endl;
	}

}

void GeneratoreLivelli::popolaLivello (Livello* l){

	int maxLink = 0;
	inizializzaVettColl(l);
	stampaCollegamenti(l);
	int stanze = l->getNStanze();
	if (stanze != 1) {
		float hard = .3;
		maxLink = stanze + (int)(hard * stanze);
	}
	printf("\tmLink: %d \t stanze: %d \n", maxLink, stanze );

	riempiVettStanze(l);
	riempiVettCollegamenti(l, maxLink);

}


