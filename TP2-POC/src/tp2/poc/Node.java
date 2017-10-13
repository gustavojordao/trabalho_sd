/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package tp2.poc;

/**
 *
 * @author 123437
 */
public class Node {
    
    private int indice;
    private int qtdNodes;
    
    private String ip_ant;
    private int porta_ant;
    private int porta_suc;
    
    private Servidor sucessor;
    private Cliente antecessor;

    public Node(int porta_suc){
        this.indice = 0;
        this.qtdNodes = 1;
        
        this.porta_suc = porta_suc;
        
        antecessor = null;
        sucessor = new Servidor(porta_suc);
    }

    public Node(String ip_ant, int porta_ant, int porta_suc){
        this.indice = 0;
        this.qtdNodes = 1;

        this.ip_ant = ip_ant;
        this.porta_ant = porta_ant;
        this.porta_suc = porta_suc;
        
        antecessor = new Cliente(ip_ant, porta_ant);
        sucessor = new Servidor(porta_suc);
    }
    
    public Servidor getSucessor() {
        return sucessor;
    }

    public void setSucessor(Servidor sucessor) {
        this.sucessor = sucessor;
    }

    public Cliente getAntecessor() {
        return antecessor;
    }

    public void setAntecessor(Cliente antecessor) {
        this.antecessor = antecessor;
    }

    public int getIndice() {
        return indice;
    }

    public void setIndice(int indice) {
        this.indice = indice;
    }

    public int getQtdNodes() {
        return qtdNodes;
    }

    public void setQtdNodes(int qtdNodes) {
        this.qtdNodes = qtdNodes;
    }

    public String getIp_ant() {
        return ip_ant;
    }

    public void setIp_ant(String ip_ant) {
        this.ip_ant = ip_ant;
    }

    public int getPorta_ant() {
        return porta_ant;
    }

    public void setPorta_ant(int porta_ant) {
        this.porta_ant = porta_ant;
    }

    public int getPorta_suc() {
        return porta_suc;
    }

    public void setPorta_suc(int porta_suc) {
        this.porta_suc = porta_suc;
    }
    
    
    
    
}
