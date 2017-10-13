/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package tp2.poc;

import java.io.Serializable;

/**
 *
 * @author 123437
 */
public class Mensagem implements Serializable{
    
    private int codigo;
    private String texto;
    
    public Mensagem(){
        this.codigo = 1;
        this.texto = "Mensagem Teste";
    }

    public Mensagem(String mensagem){
        String[] partes = mensagem.split("|");
        this.codigo = Integer.parseInt(partes[0]);
        
        for(int i=1; i<partes.length; i++){
            this.texto = "|" + partes[i];
        }
        
        this.texto = this.texto.substring(1);
    }
    
    public Mensagem(int codigo, String texto){
        this.codigo = codigo;
        this.texto = texto;
    }
    
    public int getCodigo() {
        return codigo;
    }

    public void setCodigo(int codigo) {
        this.codigo = codigo;
    }

    public String getTexto() {
        return texto;
    }

    public void setTexto(String texto) {
        this.texto = texto;
    }

    @Override
    public String toString() {
        return this.codigo + "|" + this.texto;
    }
    
    public static Mensagem mensagemSolicitacaoPorta(){
        return new Mensagem(2, "");
    }
    
    public static Mensagem mensagemRespostaSolicitacaoPorta(int porta){
        return new Mensagem(3, Integer.toString(porta));
    }
}
