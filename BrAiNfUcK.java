import java.io.*;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.stream.Stream;
import java.util.Arrays;

public class BrAiNfUcK{

        static boolean DEBUG = false;
        static String infile;
        static int[] band;
        static int cursor = 0, cursor_offset = 0;
        
        public static void main(String[] args) {
                
                for(String param : args){

                        if(param.equalsIgnoreCase("-d")){
                                DEBUG = true;
                        }else{
                                // Let's use this shit as file
                                infile = param;
                        }
                }
                
                if(DEBUG){
                        System.out.println("INIT");
                }
                // Init array
                band = new int[1];
                band[0] = 0;
                String brainfuck = "";

                try{
                        brainfuck = new String ( Files.readAllBytes( Paths.get(infile) ) );
                }catch(Exception e){
                        e.printStackTrace();
                        System.exit(1);
                }
                
                if(DEBUG){
                        System.out.println(brainfuck);
                }

                for(int i = 0; i < brainfuck.length(); i++){
                        // Iterate through the string elements
                        if(DEBUG){
                                System.out.println("ITERATOR " + i + "; CURSOR: " + cursor + "; CURSOR_OFFSET:" + cursor_offset);
                        }
                        
                        switch(brainfuck.charAt(i)){
                        case '+':
                                band[cursor + cursor_offset]++;
                                
                                if(DEBUG){
                                        System.out.println("ALTERING BAND AT POSITION " + (cursor + cursor_offset) + " to " + band[cursor+cursor_offset]);
                                
                                }
                                break;
                        case '-':
                                band[cursor + cursor_offset]--;
                                
                                if(DEBUG){
                                        System.out.println("ALTERING BAND AT POSITION " + (cursor + cursor_offset) + " to " + band[cursor+cursor_offset]);
                                }
                                
                                break;
                        case '.':
                                System.out.print((char)band[cursor + cursor_offset]);
                                System.out.flush();
                                break;
                        case ',':
                                System.out.println("TODO: implement input");
                                break;
                        case '>':
                                // Increment the cursor position by 1
                                cursor++;
                                if(cursor+cursor_offset >= band.length){
                                        band = Arrays.copyOf(band,band.length+1);
                                        // Adds 0s so i don't care
                                 }
                                 break;
                        case '<':
                                cursor --;
                                if(cursor + cursor_offset < 0){
                                        int[] band_tmp = new int[band.length + 1];
                                        cursor_offset++;
                                        
                                        for(int ii = 0; ii < band_tmp.length; ii++){
                                                band_tmp[ii] = band[ii];
                                        }
                                        
                                        band = band_tmp;
                                        band[0] = 0; // null the new element
                                }

                                break;
                        case '[':
                                if(band[cursor + cursor_offset] == 0){
                                        // Find my ]
                                        int out = 1;
                                        for(int ii = i+1; ii < brainfuck.length(); ii++){
                                                if(brainfuck.charAt(ii) == '['){
                                                        out++;
                                                }
                                                
                                                if(brainfuck.charAt(ii) == ']'){
                                                        out--;
                                                }

                                                if(out == 0){
                                                        i = ii;
                                                        break;
                                                }
                                                
                                        }
                                } // Else just continue on
                                break;
                        case ']':

                                if(band[cursor + cursor_offset] != 0){
                                        // Find my [
                                        int out = 1;
                                        for(int ii = i-1; ii >= 0; ii--){

                                                if(brainfuck.charAt(ii) == '['){
                                                        out--;
                                                }
                                                
                                                if(brainfuck.charAt(ii) == ']'){
                                                        out++;
                                                }

                                                if(out == 0){
                                                        i = ii;
                                                        break;
                                                }
                                                
                                        }
                                } // Else just continue on
                                break;
                        }

                }

                return;

        }

}
