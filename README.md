## Instruções para Compilação e Execução

1. **Compilação**

   - Certifique-se de que o [Makefile](Makefile) está no diretório principal do projeto.
   - Abra um terminal no diretório do projeto.
   - Execute o comando:
     ```bash
     make
     ```
   - Isso compilará o programa e criará o executável chamado `program`.

2. **Execução**

   - Após a compilação, você pode executar o programa com o comando:
     ```bash
     ./program
     ```
   - O programa irá ler o arquivo `teste.txt` (ou qualquer arquivo especificado no código) e processar as postagens.

3. **Limpeza**

   - Para remover o executável gerado e limpar os arquivos temporários, execute:
     ```bash
     make clean
     ```

## Arquivos do Projeto

- `sources/main.c`: Código fonte principal.
- `sources/ArvoreAVL.c`: Implementação da árvore AVL.
- `sources/ArvoreAVL_set.c`: Implementação da árvore AVL para uso das estruturas do tipo Set
- `Makefile`: Arquivo de configuração para compilação.

## Dependências

- **gcc**: Certifique-se de ter o compilador GCC instalado no seu sistema.
