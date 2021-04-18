package de.kai_morich.simple_bluetooth_le_terminal;

import android.os.Bundle;

import androidx.annotation.Nullable;
import androidx.fragment.app.DialogFragment;
import androidx.fragment.app.Fragment;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.widget.Button;

import com.google.android.material.textfield.TextInputEditText;

/**
 * A simple {@link Fragment} subclass.
 * Use the {@link EnterDistanceFragment#newInstance} factory method to
 * create an instance of this fragment.
 */
public class EnterDistanceFragment extends DialogFragment {



    public EnterDistanceFragment() {
        // Required empty public constructor
    }
    private TextInputEditText editText;

    public static EnterDistanceFragment newInstance() {
        EnterDistanceFragment fragment = new EnterDistanceFragment();
        return fragment;
    }


    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        return inflater.inflate(R.layout.fragment_enter_distance, container, false);
    }

    @Override
    public void onViewCreated(View view, @Nullable Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        editText = view.findViewById(R.id.dist);
        editText.requestFocus();
        getDialog().getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_STATE_ALWAYS_VISIBLE);
        Button okbtn = view.findViewById(R.id.btnok);
        okbtn.setOnClickListener(v -> onOK());
        Button kclbtn = view.findViewById(R.id.btncancel);
        kclbtn.setOnClickListener(v -> onCancel());
    }

    public void onOK(){
        EnterDistanceFragmentListener list = (EnterDistanceFragmentListener) getTargetFragment();
        list.enteredDistance(Float.parseFloat((editText.getText().toString())));
        dismiss();

    }
    public void onCancel(){
        dismiss();
    }
}