package com.example.aplicacionprueba;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.Toast;

import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;

public class MainActivity2 extends AppCompatActivity {

    private EditText nombreM;
    private String nombreMas;

    private Spinner peso;
    private Spinner edad;
    private Spinner proporciones;

    private String[] PesoL = {"2Kg", "3Kg", "4Kg", "5Kg", "8Kg", "10Kg", "15Kg", "20Kg", "30Kg", "40Kg", "50Kg", "+60Kg"};
    private String[] EdadL = {"Cachorro","Joven","Adulto","Viejo"};
    private String[] ProporcionesL = {"1", "2", "3"};

    FirebaseDatabase database = FirebaseDatabase.getInstance();
    DatabaseReference referenceEntrada1 = database.getReference("Peso");
    DatabaseReference referenceEntrada2 = database.getReference("Proporciones");
    DatabaseReference referenceEntrada3 = database.getReference("Nombre");
    /*DatabaseReference referenceEntrada4 = database.getReference("HorarioT");
    DatabaseReference referenceEntrada5 = database.getReference("HorarioN");
    */

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main2);

        nombreM=findViewById(R.id.nombreDeMascota);

        peso = findViewById(R.id.listaDePeso);
        ArrayAdapter<String> adapta1 = new ArrayAdapter<>(this, android.R.layout.simple_spinner_dropdown_item, PesoL);
        peso.setAdapter(adapta1);

        edad = findViewById(R.id.listaEdad);
        ArrayAdapter<String> adapta2 = new ArrayAdapter<>(this, android.R.layout.simple_spinner_dropdown_item, EdadL);
        edad.setAdapter(adapta2);

        proporciones = findViewById(R.id.listaDeProporciones);
        ArrayAdapter<String> adapta3 = new ArrayAdapter<>(this, android.R.layout.simple_spinner_dropdown_item, ProporcionesL);
        proporciones.setAdapter(adapta3);

        nombreMas = nombreM.getText().toString();

    }

    public void Enviar(View view){
        Intent i = new Intent(this,MainActivity.class);
        i.putExtra("nombreMasc",nombreM.getText().toString());

        Toast.makeText(this, "Configuracion aplicada", Toast.LENGTH_SHORT).show();

        String pesoSeleccionado = peso.getSelectedItem().toString();
        String proporcionesSeleccionadas = proporciones.getSelectedItem().toString();

        switch (pesoSeleccionado) {
            case "2Kg":
                referenceEntrada1.setValue(2);

                break;
            case "3Kg":
                referenceEntrada1.setValue(3);

                break;
            case "4Kg":
                referenceEntrada1.setValue(4);

                break;
            case "5Kg":
                referenceEntrada1.setValue(5);

                break;
            case "8Kg":
                referenceEntrada1.setValue(8);

                break;
            case "10Kg":
                referenceEntrada1.setValue(10);

                break;
            case "15Kg":
                referenceEntrada1.setValue(15);

                break;
            case "20Kg":
                referenceEntrada1.setValue(20);

                break;
            case "30Kg":
                referenceEntrada1.setValue(30);

                break;
            case "40Kg":
                referenceEntrada1.setValue(40);

                break;
            case "50Kg":
                referenceEntrada1.setValue(50);

                break;
            case "+60Kg":
                referenceEntrada1.setValue(60);

                break;
            default:
                throw new IllegalStateException("Unexpected value: " + pesoSeleccionado);
        }

        switch (proporcionesSeleccionadas) {
            case "1":
                referenceEntrada2.setValue(1);

                break;
            case "2":
                referenceEntrada2.setValue(2);

                break;
            case "3":
                referenceEntrada2.setValue(3);

                break;
            default:
                throw new IllegalStateException("Unexpected value: " + proporcionesSeleccionadas);
        }

        referenceEntrada3.setValue(nombreM.getText().toString());


        startActivity(i);
    }

    @Override
    protected void onSaveInstanceState(@NonNull Bundle outState) {
        super.onSaveInstanceState(outState);
        outState.putString("nombreMas",nombreMas);

    }

    @Override
    protected void onRestoreInstanceState(@NonNull Bundle savedInstanceState) {
        super.onRestoreInstanceState(savedInstanceState);
        nombreMas = savedInstanceState.getString("nombreMas");
        nombreM.setText(String.valueOf(nombreMas));
    }

}