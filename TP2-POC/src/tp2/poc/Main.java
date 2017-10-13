/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package tp2.poc;

import java.util.Scanner;

/**
 *
 * @author 123437
 */
public class Main {

    public static Node node;
    public static Thread t_cli_recv = new Thread(new ClienteRecebeMensagem());
    public static Thread t_srv_recv = new Thread(new ServidorRecebeMensagem());
    public static Thread t_srv_accept = new Thread(new ServidorAceitaCliente());
        
    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        // TODO code application logic here
        
        t_cli_recv = new Thread(new ClienteRecebeMensagem());
        t_srv_recv = new Thread(new ServidorRecebeMensagem());
        t_srv_accept = new Thread(new ServidorAceitaCliente());
               
        node = null;
        
        if(args.length >= 3){
            node = new Node(args[0], Integer.valueOf(args[1]), Integer.valueOf(args[2]));
        }
        else{
            node = new Node(Integer.valueOf(args[0]));
        }
        
        node.getSucessor().iniciar();
        
        if(node.getAntecessor() != null){
            node.getAntecessor().conectar();
            t_cli_recv.start();
        }
        
        t_srv_accept.start();
        
        while(true){
            Scanner sc = new Scanner(System.in);
            
            int msg = sc.nextInt();
            
            switch(msg){
                case 11:
                    node.getAntecessor().enviar(new Mensagem());
                    break;
                case 12:
                    node.getSucessor().enviar(new Mensagem());
                    break;
            }
        }
    }
    
}
