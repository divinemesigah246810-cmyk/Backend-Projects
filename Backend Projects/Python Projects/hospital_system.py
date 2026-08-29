from datetime import datetime

class Patient:
    def __init__(self, patient_id, full_name, dob, gender, phone, email, address):
        self.patient_id = patient_id
        self.full_name = full_name
        self.dob = dob
        self.gender = gender
        self.phone = phone
        self.email = email
        self.address = address


class Doctor:
    def __init__(self, doctor_id, full_name, specialization, phone):
        self.doctor_id = doctor_id
        self.full_name = full_name
        self.specialization = specialization
        self.phone = phone


class Appointment:
    def __init__(self, appointment_id, patient_id, doctor_id, date_str, status="Scheduled"):
        self.appointment_id = appointment_id
        self.patient_id = patient_id
        self.doctor_id = doctor_id
        self.date_str = date_str  # Format: "DD/MM/YYYY"
        self.status = status      # Scheduled, Completed, Cancelled


class MedicalRecord:
    def __init__(self, record_id, patient_id, doctor_id, diagnosis, prescription):
        self.record_id = record_id
        self.patient_id = patient_id
        self.doctor_id = doctor_id
        self.diagnosis = diagnosis
        self.prescription = prescription
        self.date = datetime.now().strftime("%d/%m/%Y")


class HospitalManagementSystem:
    def __init__(self, hospital_name="City Central Hospital"):
        self.hospital_name = hospital_name
        self.patients = {}      # patient_id -> Patient
        self.doctors = {}       # doctor_id -> Doctor
        self.appointments = {}  # appointment_id -> Appointment
        self.records = []       # List of MedicalRecord objects
        
        self.pat_id_counter = 100
        self.doc_id_counter = 500
        self.app_id_counter = 1000
        self.rec_id_counter = 5000

    # --- PATIENT MANAGEMENT ---
    def register_patient(self, name, dob, gender, phone, email, address):
        pat_id = f"PAT-{self.pat_id_counter}"
        self.pat_id_counter += 1
        patient = Patient(pat_id, name, dob, gender, phone, email, address)
        self.patients[pat_id] = patient
        print(f"Patient registered successfully! Patient ID: {pat_id}")
        return pat_id

    def view_patient(self, patient_id):
        patient = self.patients.get(patient_id)
        if not patient:
            print("Patient not found.")
            return
        print(f"\n--- Patient Info ({patient_id}) ---")
        print(f"Name: {patient.full_name}\nDOB: {patient.dob} ({patient.gender})")
        print(f"Phone: {patient.phone}\nEmail: {patient.email}\nAddress: {patient.address}")

    # --- DOCTOR MANAGEMENT ---
    def register_doctor(self, name, specialization, phone):
        doc_id = f"DOC-{self.doc_id_counter}"
        self.doc_id_counter += 1
        doctor = Doctor(doc_id, name, specialization, phone)
        self.doctors[doc_id] = doctor
        print(f"Doctor registered successfully! Doctor ID: {doc_id}")
        return doc_id

    # --- APPOINTMENTS ---
    def book_appointment(self, patient_id, doctor_id, date_str):
        if patient_id not in self.patients:
            print("Error: Patient ID not found.")
            return None
        if doctor_id not in self.doctors:
            print("Error: Doctor ID not found.")
            return None

        app_id = f"APP-{self.app_id_counter}"
        self.app_id_counter += 1
        appointment = Appointment(app_id, patient_id, doctor_id, date_str)
        self.appointments[app_id] = appointment

        patient_name = self.patients[patient_id].full_name
        doctor_name = self.doctors[doctor_id].full_name
        print(f"\nAppointment Booked! ID: {app_id}")
        print(f"Patient: {patient_name} | Doctor: {doctor_name} | Date: {date_str}\n")
        return app_id

    def update_appointment_status(self, appointment_id, status):
        appointment = self.appointments.get(appointment_id)
        if not appointment:
            print("Appointment not found.")
            return
        if status in ["Scheduled", "Completed", "Cancelled"]:
            appointment.status = status
            print(f"Appointment {appointment_id} status updated to: {status}")
        else:
            print("Invalid status.")

    # --- MEDICAL RECORDS ---
    def add_medical_record(self, patient_id, doctor_id, diagnosis, prescription):
        if patient_id not in self.patients or doctor_id not in self.doctors:
            print("Error: Invalid Patient or Doctor ID.")
            return
        rec_id = f"REC-{self.rec_id_counter}"
        self.rec_id_counter += 1
        record = MedicalRecord(rec_id, patient_id, doctor_id, diagnosis, prescription)
        self.records.append(record)
        print(f"Medical record added successfully for Patient {patient_id}.")

    # --- REPORTS & STATEMENTS ---
    def print_patient_history(self, patient_id):
        patient = self.patients.get(patient_id)
        if not patient:
            print("Patient not found.")
            return

        print("=" * 55)
        print(f"{self.hospital_name.upper()} - MEDICAL HISTORY")
        print("=" * 55)
        print(f"Patient Name : {patient.full_name}")
        print(f"Patient ID   : {patient.patient_id}")
        print(f"Contact      : {patient.phone}")
        print("-" * 55)
        print("DIAGNOSIS & PRESCRIPTION HISTORY")
        print("-" * 55)

        p_records = [r for r in self.records if r.patient_id == patient_id]
        if not p_records:
            print("No medical records found.")
        else:
            for r in p_records:
                doc_name = self.doctors[r.doctor_id].full_name if r.doctor_id in self.doctors else "Unknown"
                print(f"Date        : {r.date}")
                print(f"Doctor      : {doc_name}")
                print(f"Diagnosis   : {r.diagnosis}")
                print(f"Prescription: {r.prescription}")
                print("-" * 55)
        print("=" * 55 + "\n")


# --- RUNTIME DEMONSTRATION ---
if __name__ == "__main__":
    hms = HospitalManagementSystem("St. Jude Community Hospital")

    # 1. Register Staff and Patients
    p_id = hms.register_patient("Kwame Nkrumah", "12/09/1985", "Male", "+233240000000", "kwame@example.com", "Accra")
    d_id = hms.register_doctor("Dr. Abena Mensah", "Cardiology", "+233270000000")

    # 2. Book Appointment
    app_id = hms.book_appointment(p_id, d_id, "28/08/2026")

    # 3. Doctor Consultation & Medical Record Entry
    hms.update_appointment_status(app_id, "Completed")
    hms.add_medical_record(
        patient_id=p_id,
        doctor_id=d_id,
        diagnosis="Mild Hypertension",
        prescription="Amlodipine 5mg daily, Low sodium diet"
    )

    # 4. Generate Medical Report
    hms.print_patient_history(p_id)