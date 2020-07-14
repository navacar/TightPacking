#include <QCoreApplication>
#include <QVector>
#include <QList>
#include <iostream>
#include <QRandomGenerator>

using namespace :: std;

const int MaxMemory = 1000000;
QList<int> files; /* = {484802, 420713, 396295, 392019, 262926, 254274,
                    230973, 204065, 324387, 320414, 311011, 347661,
                    347560, 286309, 340190, 331812, 326585, 383972,
                    351543, 263485};*/
int Disks = 0;
QVector<QVector<int>> StoreForOutput;

void FileCreator(int N, int Times)
{
    int Rand = 0;
    QRandomGenerator randGen;
    for(int j = 0; j < Times; j++){
        for (int i = 0; i < N; i++){
            Rand = randGen.bounded(MaxMemory + 1);
            if(j == Times - 1)
               files.append(Rand);
        }
    }
}

int Sorting(int N, int SortedFiles[N])
{
    for(int i = 0; i < N; i++){
        SortedFiles[i] = files.at(i);
    }

    int t = 0;
    for(int i = 0; i < N-1; i ++){
        for (int j = i + 1; j < N; j++){
            if(SortedFiles[i] < SortedFiles[j]){
                t = SortedFiles[i];
                SortedFiles[i] = SortedFiles[j];
                SortedFiles[j] = t;
            }
        }
    }

    return SortedFiles[N];
}

void OutPut()
{
    QVector<QVector<int>> Store = StoreForOutput;
    QVector<int> DiskFromStore;
        int Total = 0;
        for(int i = 0; i < Store.size(); i++){
            DiskFromStore = Store.at(i);
            cout << "Диск " << i <<": ";
            for(int j = 0; j < DiskFromStore.size(); j++){
                cout << DiskFromStore.at(j);
                Total += DiskFromStore.at(j);
                if(j + 1 == DiskFromStore.size())
                    cout << ": " << "Свободно :" << MaxMemory - Total << endl;
                else
                    cout << ", ";
            }
            Total = 0;
        }
}

void Heuristic1(int N){
    Disks = 0;
    QVector<QVector<int>> Store;
    QVector<int> NewDisk, DiskFromStore;
    int SortedFiles[1000000];
    SortedFiles[N] = Sorting(N, SortedFiles);
    int UsedMemory = 0;
    for(int i = 0; i < N; i++){
        if(UsedMemory + SortedFiles[i]> MaxMemory){

            Store.append(NewDisk);
            NewDisk.clear();
            UsedMemory = SortedFiles[i];
            NewDisk.append(SortedFiles[i]);
        }else{
            NewDisk.append(SortedFiles[i]);
            UsedMemory += SortedFiles[i];
        }
    }

    if(!NewDisk.isEmpty())
        for(int i = 0; i < NewDisk.size(); i++){
            UsedMemory = 0;
            DiskFromStore = Store.at(i);
            for(int j = 0; j < DiskFromStore.size(); j++){
                UsedMemory += DiskFromStore.at(j);
            }
            if(UsedMemory + NewDisk.at(i) < MaxMemory){
                DiskFromStore.append(NewDisk.at(i));
            }

        }
        Store.append(NewDisk);

    Disks = Store.size();
    cout << "Heuristic 1: " << Disks << endl;

    StoreForOutput = Store;
    //OutPut();

}

void Heuristic2(int N)
{
    Disks = 0;
    QVector<QVector<int>> Store;
    QVector<int> NewDisk, DiskFromStore;
    int SortedFiles[1000000];
    SortedFiles[N] = Sorting(N, SortedFiles);
    for(int i = 0; i < N; i++){
        if(Store.isEmpty()){
            NewDisk.append(SortedFiles[i]);
            Store.append(NewDisk);
        }else{
            NewDisk.remove(0, NewDisk.size());
            int DiskFromStoreMemory[Store.size()];
            for(int j = 0; j < Store.size(); j ++){
                DiskFromStoreMemory[j] = 0;
            }
            for(int j = 0; j < Store.size(); j++){
                for(int k = 0; k < Store.at(j).size(); k++){
                    DiskFromStoreMemory[j] += Store.at(j).at(k);
                }
            }
            int Max = 0, MaxPosition = -1;
            bool Saved = false;
            while(!Saved){
                for(int j = 0; j < Store.size(); j ++){
                    if(DiskFromStoreMemory[j] > Max){
                        Max = DiskFromStoreMemory[j];
                        MaxPosition = j;
                    }

                    if(Max + SortedFiles[i] <= MaxMemory){
                        DiskFromStore = Store.at(MaxPosition);
                        DiskFromStore.append(SortedFiles[i]);
                        Store.replace(MaxPosition, DiskFromStore);
                        Saved = true;
                    }else{
                        Max = 0;

                        DiskFromStoreMemory[MaxPosition] = 0;
                        MaxPosition = 0;
                    }
                    int UsedMemory = 0;
                    for(int k = 0; k < Store.size(); k++)
                        UsedMemory += DiskFromStoreMemory[k];
                    if(UsedMemory == 0){
                        NewDisk.append(SortedFiles[i]);
                        Store.append(NewDisk);
                        Saved = true;
                    }
                    if(Saved == true)
                        break;
                }

            }

        }
    }

    Disks = Store.size();
    cout << "Heuristic 2: " << Disks << endl;

    StoreForOutput = Store;
    //OutPut();
}

int main()
{
    int N;
    cout << "Введите количество файлов: ";
    cin >> N;

    for(int i = 0; i < 100; i++){
        FileCreator(N, i + 1);
        cout<< "===================" << endl;
        Heuristic1(N);
        Heuristic2(N);
        cout<< "===================" << endl;
        files.clear();
    }

    return 0;
}
