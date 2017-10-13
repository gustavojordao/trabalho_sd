/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package tp2.poc;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.Socket;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author 123437
 */
public class Cliente {
    
    private String ip;
    private int porta;
    private Socket socket;
    
    public Cliente(String ip, int porta){
        this.ip = ip;
        this.porta = porta;
    }
    
    public void conectar(){
        try{
            socket = new Socket(ip, porta);
            System.out.println("Conectou ao servidor: "+ip+":"+porta);
        }
        catch(IOException ex){
            System.out.println("Falha ao conectar com servidor.");
            ex.printStackTrace();
            socket = null;
        }
        
    }
    
    public void enviar(Mensagem m){
        ObjectOutputStream saida = null;
        try {
            //while(socket == null);
            System.out.print("Cliente enviando "+m.toString()+"...");
            
            saida = new ObjectOutputStream(socket.getOutputStream());
            
            //saida.writeUTF(m.toString());
            saida.writeObject(m);
            
            saida.flush();
            System.out.println("Ok");

        } catch (IOException ex) {
            System.out.println("Falha ao enviar mensagem ao servidor.");
            ex.printStackTrace();
        }  /*finally {
            try {
                saida.close();
            } catch (IOException ex) {
                //System.out.println("Falha ao encerrar conexão de saída do cliente.");
                //ex.printStackTrace();
            }
        }*/
    }
    
    public Mensagem receber() {
        
        Mensagem m = null;
        ObjectInputStream entrada = null;
        try {
            while(socket == null);
            
            System.out.print("Cliente recebendo...");
            entrada = new ObjectInputStream(socket.getInputStream());
            
            //String mensagem = entrada.readUTF();
            //m = new Mensagem(mensagem);
            m = (Mensagem) entrada.readObject();
            
            System.out.println("Ok");

        } catch (IOException ex) {
            System.out.println("Falha ao receber mensagem do cliente.");
            ex.printStackTrace();
        } catch (ClassNotFoundException ex) { 
            Logger.getLogger(Cliente.class.getName()).log(Level.SEVERE, null, ex);
        }
        /*finally{
            try {
                entrada.close();
            } catch (IOException ex) {
                //System.out.println("Falha ao encerrar conexão de entrada do cliente.");
                //ex.printStackTrace();
            }
        }*/
        return m;
    }
    
}
