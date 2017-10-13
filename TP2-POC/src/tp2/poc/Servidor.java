/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package tp2.poc;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketAddress;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author 123437
 */
public class Servidor {
    
    private int porta;
    private ServerSocket ss;
    private Socket cliente;
    private Socket novoCliente;
    
    public Servidor(int porta){
        this.porta = porta;
    }
    
    public void iniciar(){
        try{
            ss = new ServerSocket(porta);
        }
        catch(IOException ex){
            System.out.println("Falha ao iniciar servidor.");
            ex.printStackTrace();
            ss = null;
        }
        
    }
    
    public void aceitar(){
        try {
            novoCliente = ss.accept();
            
            if(cliente == null){
                cliente = novoCliente;
            }
            else{
                // TODO: Lógica de reorganização
            }
            
            System.out.println("Aceitou conexão de cliente: "+getIpCliente()+":"+getPortaCliente());
        } catch (IOException ex) {
            System.out.println("Falha ao aceitar cliente no servidor.");
            ex.printStackTrace();
        }
    }
    
    public void enviar(Mensagem m){
        ObjectOutputStream saida = null;
        try {
            //while(cliente == null);
            System.out.print("Servidor enviando "+m.toString()+"...");

            saida = new ObjectOutputStream(cliente.getOutputStream());
            
            //saida.writeUTF(m.toString());
            saida.writeObject(m);
            
            saida.flush();
            System.out.println("Ok");

        } catch (IOException ex) {
            System.out.println("Falha ao enviar mensagem ao cliente.");
            ex.printStackTrace();
        } /*finally {
            try {
                saida.close();
            } catch (IOException ex) {
                //System.out.println("Falha ao encerrar conexão de saída do servidor.");
                //ex.printStackTrace();
            }
        }*/
    }
    
    public Mensagem receber() {
        
        Mensagem m = null;
        ObjectInputStream entrada = null;
        try {
            while(cliente == null);
            
            System.out.print("Servidor recebendo...");
            
            entrada = new ObjectInputStream(cliente.getInputStream());
            
            //String mensagem = entrada.readUTF();
            //m = new Mensagem(mensagem);
            m = (Mensagem) entrada.readObject();
            
            System.out.println("Ok");
            
        } catch (IOException ex) {
            System.out.println("Falha ao receber mensagem do cliente.");
            ex.printStackTrace();
        } catch (ClassNotFoundException ex) { 
            Logger.getLogger(Servidor.class.getName()).log(Level.SEVERE, null, ex);
        }
        /*finally{
            try {
                entrada.close();
            } catch (IOException ex) {
                //System.out.println("Falha ao encerrar conexão de entrada do servidor.");
                //ex.printStackTrace();
            }
        }*/
        return m;
    }
    
    public String getIpCliente(){
        return cliente.getInetAddress().getHostAddress();
    }
    
    public int getPortaCliente(){
        return cliente.getLocalPort();
    }
}
