//Paquetes y librerias:
package com.example.aplicacionprueba;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

import java.util.HashMap;

//Codigo:
public class MainActivity extends AppCompatActivity {

    FirebaseDatabase database = FirebaseDatabase.getInstance();
    DatabaseReference referenceEntrada1 = database.getReference("Nombre");
    DatabaseReference referenceEntrada2 = database.getReference("Conexion");

    private TextView nombreTitulo;
    private TextView Conexion;

    private String nombreReci;

    private String estadoConexion="Desconectado";


    @Override
    protected void onCreate(Bundle savedInstanceState) {    //Referencias hacia activity_main.xml
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        nombreTitulo = findViewById(R.id.nombreRecibido);
        Conexion = findViewById(R.id.conexion);

        nombreReci=getIntent().getStringExtra("nombreMasc");
        nombreTitulo.setText(nombreReci);

        Conexion.setText(estadoConexion);

        referenceEntrada1.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot snapshot) {
                nombreReci = snapshot.getValue().toString();
                nombreTitulo.setText(nombreReci);
            }

            @Override
            public void onCancelled(@NonNull DatabaseError error) {

            }
        });

        referenceEntrada2.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot snapshot) {
                String C = snapshot.getValue().toString();
                Conexion.setText(C);
            }

            @Override
            public void onCancelled(@NonNull DatabaseError error) {

            }
        });

    }


    @Override
    protected void onSaveInstanceState(@NonNull Bundle outState) {
        super.onSaveInstanceState(outState);
        outState.putString("nombreReci",nombreReci);
        nombreTitulo.setText(String.valueOf(nombreReci));
    }

    @Override
    protected void onRestoreInstanceState(@NonNull Bundle savedInstanceState) {
        super.onRestoreInstanceState(savedInstanceState);
        nombreReci = savedInstanceState.getString("nombreReci");
        nombreTitulo.setText(String.valueOf(nombreReci));
    }




    @Override
    public boolean onOptionsItemSelected(@NonNull MenuItem item) {
        Intent intento=new Intent(this,MainActivity2.class);
        Intent intento2=new Intent(this,MainActivity3.class);

        int num=item.getItemId();
        switch(num){
            case R.id.Mascota:
                startActivity(intento);

                return true;
            case R.id.Preferencias:
                startActivity(intento2);
                return true;
            case R.id.Historial:
                Toast.makeText(this, "No disponible", Toast.LENGTH_SHORT).show();
                return true;

                default:return super.onOptionsItemSelected(item);

        }

    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.menu1,menu);
        return true;
    }

}