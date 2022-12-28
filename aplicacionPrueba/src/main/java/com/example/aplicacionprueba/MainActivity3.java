package com.example.aplicacionprueba;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

import java.util.HashMap;

public class MainActivity3 extends AppCompatActivity {

    TextView Temperatura;
    TextView Humedad;
    TextView Luz;
    Button botonLeds;
    boolean estado1=false ;
    String estado;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main3);
        botonLeds=findViewById(R.id.botonLeds);

        Temperatura=findViewById(R.id.temperatura);
        Humedad=findViewById(R.id.humedad);
        Luz=findViewById(R.id.luz);

        FirebaseDatabase database = FirebaseDatabase.getInstance();
        DatabaseReference referenceEntrada1= database.getReference("Leds");

        DatabaseReference referenceEntrada2= database.getReference("DHT");
        DatabaseReference referenceEntrada3= database.getReference("Luz");

        referenceEntrada1.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot snapshot) {
                estado = snapshot.getValue().toString();

                if(estado.equals("true")){
                    botonLeds.setText("Encendido");
                }else if(estado.equals("false")){
                    botonLeds.setText("Apagado");
                }
            }


            @Override
            public void onCancelled(@NonNull DatabaseError error) {

            }
        });

        botonLeds.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                //HashMap<Object,Object>info = new HashMap<>();
                estado1 = !estado1;
                //info.put("Leds",estado1);
                referenceEntrada1.setValue(estado1);
            }
        });

        referenceEntrada2.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot snapshot) {
                String T = snapshot.child("Temp").getValue().toString();
                Temperatura.setText(T);
                String H = snapshot.child("Hum").getValue().toString();
                Humedad.setText(H);

            }

            @Override
            public void onCancelled(@NonNull DatabaseError error) {

            }
        });

        referenceEntrada3.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot snapshot) {
                String L = snapshot.getValue().toString();
                Luz.setText(L);
            }

            @Override
            public void onCancelled(@NonNull DatabaseError error) {

            }
        });

    }
}