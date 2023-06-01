#include <stdio.h>

typedef struct{
  char nome[50];
  double  saldo;
  int  codigo;
}Contas;

int adicionar(Contas aux);
int txt(Contas aux); 
int duplicidade(Contas aux);
int deleta(int codigo);
void inicia();
int saque(double valor, int codigo);
int deposito(double valot, int codigo);
int extrato_pessoal(int codigo);
int verifica(int codigo);
void menu();

int main(){
  Contas ptr;
  FILE *fileBin = fopen("contas.bin","ab");
 
  
  int opc = 0;
  int c;
  double val;
  while(opc!=6){
    menu();
    printf("opcao-> ");
    scanf("%d",&opc);
    puts("");
    switch(opc){
      case(1):
        inicia(fileBin);
        adicionar(ptr);
        txt(ptr);
      break;
      case(2):
        printf("Codigo da conta-> ");
        scanf("%d",&c);
        if(verifica(c)){
          deleta(c);
          txt(ptr);
        }
        else{
          printf("Conta inexistente");
        }
        
      break;
      case(3):
        printf("Codigo da conta-> ");
        scanf("%d",&c);
        if(verifica(c)){
           printf("valor do saque-> ");
           scanf("%lf",&val);
           saque(val,c);
           txt(ptr);
        }
        else{
          printf("Conta inexistente");
        }
      break;
      case(4):
        printf("Codigo da conta-> ");
        scanf("%d",&c);
        if(verifica(c)){
           printf("valor do saque-> ");
           scanf("%lf",&val);
           deposito(val,c);
           txt(ptr);
        }
        else{
          printf("Conta inexistente");
        }
       
      break;
      case(5):
       printf("Codigo da conta-> ");
       scanf("%d",&c);
       if(verifica(c)){
         extrato_pessoal(c);
       }
       else{
          printf("Conta inexistente");
        }
       
      break;
    }
  }
  
  return 0;
}

void menu(){
   printf("\nMENU\n");
   printf("1: Adiconar conta\n");
   printf("2: Excluir conta\n");
   printf("3: Saque\n");
   printf("4: Deposito\n");
   printf("5: Extrato\n");
   printf("6: Sair\n");
}
void inicia(FILE *aux){
  Contas ptr = {"00", 0.0, 0};
  for(int i =0; i< 5;i++){
    fwrite(&ptr,sizeof(Contas),1,aux);
  }
  fclose(aux);
}
int adicionar(Contas aux){
  FILE *arq = fopen("contas.bin","rb+");
  int quant = 0;
   
  if(!arq){
    printf("ERRO: Não foi possivel abrir o arquivo\n");
    return 0;
  }
  else{
  
    printf("Nome-> ");
    scanf("%19s",aux.nome);
    printf("Saldo-> ");
    scanf("%16lf",&aux.saldo);
    printf("codigo-> ");
    scanf("%d",&aux.codigo);
    if(!duplicidade(aux)){
      fseek(arq,(aux.codigo-1)*sizeof(aux),SEEK_SET);
      fwrite(&aux,sizeof(aux),1,arq);
    }
    else{
      FILE *ptr = fopen("duplicados.txt","w");
      fprintf(ptr,"nome:%-15s  saldo:%.2f  codigo:%d\n", 
        aux.nome,aux.saldo,aux.codigo);
      
    }
    
    
    fclose(arq);
    
    
    return 1;
    
  }
}
int txt(Contas aux){
    FILE *arq = fopen("contas.bin","rb");
    FILE *arq2 = fopen("contas.txt","w");
    Contas atual;
    Contas falso = {"0", 0.0, 0};
  
  if(!arq){
    printf("ERRO: Não foi possivel abrir o arquivo\n");
    return 0;
  }
  else{
    int i = 0;
    while(!feof(arq)){
      fseek(arq,sizeof(Contas)*i,SEEK_SET);
      fread(&atual,sizeof(aux),1,arq);
      if(atual.codigo != 0){
         fprintf(arq2,"nome:%-15s  saldo:%.2f  codigo:%d\n", 
        atual.nome,atual.saldo,atual.codigo);
      }
      else{
          fprintf(arq2,"nome:%-15s  saldo:%.2f  codigo:%d\n", 
          falso.nome,falso.saldo,falso.codigo);
      }
      i++;
    } 
 
    fclose(arq);
    fclose(arq2);
    return 1;
  }
}
int duplicidade(Contas aux){
  int i =0;
  Contas atual;
  FILE *ptr = fopen("contas.bin","rb");
  fseek(ptr,(aux.codigo-1)*sizeof(aux),SEEK_SET);
  fread(&atual,sizeof(atual),1,ptr);
  if(atual.codigo == aux.codigo){
    fclose(ptr);
    return 1;
  }
  else{
    fclose(ptr);
    return 0;
  }
  
}
int deleta(int codigo){
  FILE *ptr = fopen("contas.bin","rb+");
  FILE *lixo = fopen("lixeira.bin","ab");
  Contas atual;
  Contas falso = {"00", 0.0, 0};
  /////////////////////////////////////////////////////////////
  fseek(ptr,(codigo-1)*sizeof(Contas),SEEK_SET);
  fread(&atual,sizeof(atual),1,ptr); ////joga o deletado numa lixeira
  if(atual.codigo == 0){
    printf("Conta invalida");
    return 0;
  }
  fwrite(&atual,(atual.codigo-1)*sizeof(Contas),1,lixo);
  //////////////////////////////////////////////////////////////
  fseek(ptr,(codigo-1)*sizeof(Contas),SEEK_SET);
  fwrite(&falso,sizeof(Contas),1,ptr);
  ///////////////////////////////////////////////////////////////
  fclose(ptr);
  return 1;
}
int saque(double valor, int codigo){
  FILE *fPtr = fopen("contas.bin", "rb+");
  FILE *Ext = fopen("extrato.txt","a");
  Contas atual;
  char ope[15] = "Saque";
  fseek(fPtr,(codigo-1)*sizeof(Contas),SEEK_SET);
  fread(&atual,sizeof(Contas),1,fPtr);
  fprintf(Ext," %-15s  %.2f   %-5s       %2d\n", 
        atual.nome,valor,ope,atual.codigo);

  if(atual.saldo<valor){
    printf("Saldo insuficente\n");
    printf("vc tem %.2fl de credito",atual.saldo);
    return  0;
    
  }
  
  atual.saldo -= valor;
  fseek(fPtr,(codigo-1)*sizeof(Contas),SEEK_SET);
  fwrite(&atual,sizeof(Contas),1,fPtr);
  
  fclose(Ext);
  fclose(fPtr);
  
  return 0;
}
int deposito(double valor, int codigo){
  FILE *fPtr = fopen("contas.bin", "rb+");
  FILE *Ext = fopen("extrato.txt","a");
  char ope[15] = "Deposito";
  
  Contas atual;

  fseek(fPtr,(codigo-1)*sizeof(Contas),SEEK_SET);
  fread(&atual,sizeof(Contas),1,fPtr);
  fprintf(Ext," %-15s  %.2f   %-5s    %2d\n", 
        atual.nome,valor,ope,atual.codigo);
  
  atual.saldo += valor;
  
  fseek(fPtr,(codigo-1)*sizeof(Contas),SEEK_SET);
  fwrite(&atual,sizeof(Contas),1,fPtr);

  fclose(Ext);
  fclose(fPtr);
  
  return 0;
}
int extrato_pessoal(int codigo){
  typedef  struct{
    char nome[50];
    double valor;
    char operacao[50];
    int  codigo;
  
  }Temp;
  FILE *extrato = fopen("extrato.txt","a+");
  FILE *arq = fopen("extrato.txt","ra+");
  
  Temp atual;
  Temp ptr = {"00",0.0,"00",0};
  
  for(int i =0; i< 5;i++){
    fprintf(extrato," %-15s  %.2f   %-5s       %d\n", 
        ptr.nome,ptr.valor,ptr.operacao,ptr.codigo);
  }
  fclose(extrato);

  
  while(!feof(arq)){
    fscanf(arq,"%s  %lf   %s    %d",atual.nome,&atual.valor,atual.operacao,&atual.codigo);
    if(atual.codigo == codigo){
       printf(" nome:%-10s  valor:%.2f     operação:%5s       codigo:%2d\n", 
        atual.nome,atual.valor,atual.operacao,atual.codigo);
    }
    
  }

 fclose(arq);

  return 1;
}
int verifica(int codigo){
  FILE *arq = fopen("contas.bin","rb");
  Contas atual;
  fseek(arq,(codigo-1)*sizeof(Contas),SEEK_SET);
  fread(&atual,sizeof(atual),1,arq);
  if(atual.codigo == 0){
    return 0;
  }
  else{
    return 1;
  }
}