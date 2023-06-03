 #include <bits/stdc++.h>
using namespace std;
// int ruch=1;
uint64_t timeMilisec() {
  using namespace std::chrono;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}
struct plansza{
    int tab[7][6];
    int granice[7];
    plansza(){
        for(int i=0;i<7;i++){
            for(int j=0;j<6;j++)tab[i][j]=0;
            granice[i]=5;
        }
    }
    char zamien(int x){
        if(x==0)return '.';
        if(x==1)return 'O';
        return '#';
    }
    void print(){
        cout<<" 1 2 3 4 5 6 7 \n";
        for(int i=0;i<6;i++){
            for(int j=0;j<7;j++) cout<<"|"<<zamien(tab[j][i]);
            cout<<"|\n";
        }
        cout<<"---------------\n";
    }
    void add(int kolumna,int gracz){
        kolumna--;
        if(granice[kolumna]==-1){
            cout<<"nieprawidlowa komenda!!\nwpisz komende ponownie: ";
            cin>>kolumna;
            cout<<"\n";
            add(kolumna,gracz);
            return;
        }
        tab[kolumna][granice[kolumna]]=gracz;
        granice[kolumna]--;
    }
    void del(int kolumna){
        kolumna--;
        granice[kolumna]++;
        tab[kolumna][granice[kolumna]]=0;
    }
    int sprawdz(int x){
        x--;
        int y=this->granice[x]+1;
        int akt=x-1;
        int zmienionePole=tab[x][y];
        int ile=1;
        while(akt>=0 && zmienionePole==tab[akt][y]){
            akt--; ile++;
        }
        akt=x+1;
        while(akt<7 && zmienionePole==tab[akt][y]){
            akt++; ile++;
        }
        if(ile>=4){
            return zmienionePole;
        }akt=y-1; ile=1;
        while(akt>=0 && zmienionePole==tab[x][akt]){
            akt--; ile++;
        }
        akt=y+1;
        while(akt<6 && zmienionePole==tab[x][akt]){
            akt++; ile++;
        }
        if(ile>=4){
            return zmienionePole;
        }akt=-1;ile=1;
        while(x+akt>=0 && y+akt>=0 && zmienionePole==tab[x+akt][y+akt]){
            ile++; akt--;
        }
        akt=1;
        while(x+akt<7 && y+akt<6 && zmienionePole==tab[x+akt][y+akt]){
            ile++; akt++;
        }
        if(ile>=4){
            return zmienionePole;
        }
        akt=1;ile=1;
        while(x+akt<7 && y-akt>=0 && zmienionePole==tab[x+akt][y-akt]){
            ile++; akt++;
        }
        akt=1;
        while(y+akt<6 && x-akt>=0 && zmienionePole==tab[x-akt][y+akt]){
            ile++; akt++;
        }
        if(ile>=4){
            return zmienionePole;
        }
        bool remis=1;
        for(int i=0;i<7;i++){
            for(int j=0;j<6;j++){
                if(!tab[i][j]){
                    remis=false;
                    break;
                }
            }
            if(!remis) break;
        }
        if(remis){
            return 4;
        }
        return 0;
    }
    void merge(plansza x){
        for(int i=0;i<7;i++){
            for(int j=0;j<6;j++){
                this->tab[i][j]=x.tab[i][j];
            }
            this->granice[i]=x.granice[i];
        }
    }
};
plansza p;
int liczbaMinMax=0;
struct bot{
    int depth;
    int sym;
    int g;
    plansza mp;
    bot(int a,int b,int c){
        this->depth=a;
        this->sym=b;
        this->g=c;
    }
    int ma(int ile,int alpha,int beta){
        liczbaMinMax++;
        int maks=-sym; int wynik;
        if(ile<=depth){
            for(int i=1;i<8;i++){
                if(mp.granice[i-1]<0){
                    continue;
                }
                mp.add(i,g);
                wynik=mp.sprawdz(i);
                if(wynik==4){
                    maks=max(maks,0);
                }else if(wynik==g){
                    maks=sym;
                }
                else if(wynik==g%2+1){
                    cout<<"COS DZIWNEGO!!!\n";    
                }else{
                    wynik=mi(ile+1,alpha,beta);
                    maks=max(maks,wynik);
                    if(wynik>=beta){
                        mp.del(i);
                        return maks;
                    }
                    if(wynik>alpha) alpha=wynik;
                }
                mp.del(i);
                if(maks==sym){
                    return sym;
                }
            }
            return maks;
        }else{
            wynik=zasymuluj(g);
            return wynik;
        }
    }
    int mi(int ile,int alpha,int beta){
        liczbaMinMax++;
        int mini=sym; int wynik;
        if(ile<=depth){
            for(int i=1;i<8;i++){
                if(mp.granice[i-1]<0){
                    continue;
                }
                mp.add(i,g%2+1);
                // mp.print();
                wynik=mp.sprawdz(i);
                if(wynik==4){
                    mini=min(mini,0);
                }else if(wynik==g%2+1){
                    mini=-sym;
                }else if(wynik==g){
                    cout<<"COS DZIWNEGO2!!!!\n";
                }else{
                    wynik=ma(ile+1,alpha,beta);
                    mini=min(mini,wynik);
                    if(wynik<=alpha){
                        mp.del(i);
                        return mini;
                    }
                    if(wynik<beta) beta=wynik;
                }
                mp.del(i);
                if(mini==-sym){
                    return -sym;
                }
                // mp.print();
            }
            return mini;
        }else{
            wynik=zasymuluj(g%2+1);
            return wynik;
        }
    }
    int zasymuluj(int gracz){
        stack<int>zmiany;
        int balans=0; int wynik,kolumna;
        for(int i=0;i<sym;i++){
            wynik=0;
            while(wynik==0){
                kolumna=rand()%7+1;
                while(mp.granice[kolumna-1]<0)kolumna=rand()%7+1;
                zmiany.push(kolumna);
                mp.add(kolumna,gracz);
                // mp.print();
                wynik=mp.sprawdz(kolumna);
                gracz=gracz%2+1;
            }
            if(wynik!=4){
                if(gracz==g%2+1) balans++;
                else balans--;
            }
            // mp.print();
            wynik=mp.sprawdz(kolumna);
            while(!zmiany.empty()){
                mp.del(zmiany.top());
                // mp.print();
                zmiany.pop();
            }
            // mp.print();
            
        }
        return balans;
    }
    int wykonajRuch(){
        mp.merge(p);
        int wynik;
        int maks=-sym-1; int ak; int ruch; int alpha=-sym-1;int beta=sym+1;
        for(int i=1;i<8;i++){

            if(mp.granice[i-1]<0){
                continue;
            }
            mp.add(i,g);
            wynik=mp.sprawdz(i);
            if(wynik==4){
                ak=0;
            }else if(wynik==g){
                ak=sym;
            }else{
                ak=mi(1,alpha,beta);
            }
            mp.del(i);
            if(ak>maks){
                maks=ak;
                ruch=i;
            }
            if(ak>=beta){
                return maks;
            }
            if(ak>alpha) alpha=ak;
        }
        return ruch;
    }
};

int main(){
    srand(time(NULL));
    int gracz; int aktKolumna,wynik;
    int wygraneAlfreda=0; int wygraneManka=0; int remisy=0;
    bot alfred(6,20,2);
    bot maniek(5,50,1);
    uint64_t t,k,sumManiekT, sumAlfredT;
    k=500;
    cout<<"\n";
    bool grajDalej=true;
    p.print();
    int liczbaGier=20;
    while(liczbaGier--){
        p=plansza();
        // cout<<"Jesli chcesz zaczynac wpisz 1. w innym przypadku wpisz 2: ";
        // cin>>gracz;
        gracz=liczbaGier%2+1;
        // alfred.depth=6;
        // maniek.depth=6;
        sumManiekT = 0;
        sumAlfredT = 0;
        
        while(true){
            liczbaMinMax=0;
            if(gracz==1){
                // cout<<"wybierz kolumne: ";
                // cin>>aktKolumna;
                // cout<<"\n";
                t=timeMilisec();
                aktKolumna=maniek.wykonajRuch();
                t=timeMilisec()-t;
                sumManiekT += t;
                // cout<<"maniek wybiera kolumne "<<aktKolumna<<" w "<<t<<" milisekundach, liczba ruchów: "<<liczbaMinMax<<"\n";
                // if(t<k){
                //     maniek.depth++;
                // }
            }else{
                t=timeMilisec();
                aktKolumna=alfred.wykonajRuch();
                t=timeMilisec()-t;
                sumAlfredT += t;
                // cout<<"alfred wybiera kolumne "<<aktKolumna<<" w "<<t<<" milisekundach, liczba ruchów: "<<liczbaMinMax<<"\n";

                // if(t<k){
                //     alfred.depth++;
                // }
            }
            p.add(aktKolumna,gracz);
            wynik=p.sprawdz(aktKolumna);
            // p.print();
            if(wynik==4){
                cout<<"REMIS!!!!\n";
                remisy++;
                break;
            }
            if(wynik==gracz){
                cout<<"WYGRYWA GRACZ "<<gracz<<"!!!!\n";
                if(gracz==1) wygraneManka++;
                else wygraneAlfreda++;
                break;
            }
            gracz=gracz%2+1;
            // ruch++;
        }
        cout<<"SUM!! maniek czas "<<sumManiekT<<" milisekundach\n";
        cout<<"SUM!! alfred czas "<<sumAlfredT<<" milisekundach\n";
        // cout<<"czy chcesz grać dalej? jeśli tak wpisz 1, w przeciwnym wypadky wpisz 0: ";
        // cin>>grajDalej;

    }
    cout<<"Alfred wygral "<<wygraneAlfreda<<" razy, Maniek wygrał "<< wygraneManka<< " razy i zremisowali "<<remisy<<" razy\n";
    return 0;
}