from datetime import datetime

class Customer:
    def __init__(self, customer_id, full_name, date_of_birth, phone, email, address):
        self.customer_id = customer_id
        self.full_name = full_name
        self.date_of_birth = date_of_birth
        self.phone = phone
        self.email = email
        self.address = address


class Account:
    def __init__(self, account_number, customer_id, account_type, balance=0.0):
        self.account_number = account_number
        self.customer_id = customer_id
        self.account_type = account_type
        self.balance = float(balance)
        self.date_created = datetime.now().strftime("%d/%m/%Y")
        self.status = "Active"


class Transaction:
    def __init__(self, account_number, trans_type, amount, post_balance):
        self.date = datetime.now().strftime("%d/%m/%Y")
        self.account_number = account_number
        self.type = trans_type
        self.amount = float(amount)
        self.post_balance = float(post_balance)


class BankManagementSystem:
    def __init__(self):
        self.customers = {}     # customer_id -> Customer
        self.accounts = {}      # account_number -> Account
        self.transactions = []  # List of Transaction objects
        self.cust_id_counter = 1000
        self.acc_num_counter = 1000012345

    # --- CUSTOMER MANAGEMENT ---
    def create_customer(self, full_name, dob, phone, email, address):
        cust_id = str(self.cust_id_counter)
        self.cust_id_counter += 1
        customer = Customer(cust_id, full_name, dob, phone, email, address)
        self.customers[cust_id] = customer
        print(f"Customer created successfully! ID: {cust_id}")
        return cust_id

    def view_customer(self, customer_id):
        customer = self.customers.get(customer_id)
        if not customer:
            print("Customer not found.")
            return
        print(f"\n--- Customer Details ({customer_id}) ---")
        print(f"Name: {customer.full_name}\nDOB: {customer.date_of_birth}\nPhone: {customer.phone}")
        print(f"Email: {customer.email}\nAddress: {customer.address}")

    def update_customer(self, customer_id, full_name=None, phone=None, email=None, address=None):
        customer = self.customers.get(customer_id)
        if not customer:
            print("Customer not found.")
            return
        if full_name: customer.full_name = full_name
        if phone: customer.phone = phone
        if email: customer.email = email
        if address: customer.address = address
        print("Customer information updated successfully.")

    def delete_customer(self, customer_id):
        if customer_id in self.customers:
            del self.customers[customer_id]
            # Remove associated accounts
            acc_to_del = [acc_num for acc_num, acc in self.accounts.items() if acc.customer_id == customer_id]
            for acc_num in acc_to_del:
                del self.accounts[acc_num]
            print(f"Customer {customer_id} and associated accounts deleted.")
        else:
            print("Customer not found.")

    # --- ACCOUNT MANAGEMENT ---
    def create_account(self, customer_id, account_type, initial_balance=0.0):
        if customer_id not in self.customers:
            print("Error: Customer ID does not exist.")
            return None
        if account_type not in ["Savings", "Current"]:
            print("Error: Invalid account type. Must be 'Savings' or 'Current'.")
            return None

        acc_num = str(self.acc_num_counter)
        self.acc_num_counter += 1
        account = Account(acc_num, customer_id, account_type, initial_balance)
        self.accounts[acc_num] = account
        
        if initial_balance > 0:
            self.transactions.append(Transaction(acc_num, "Deposit", initial_balance, initial_balance))

        print(f"Account created successfully! Account Number: {acc_num}")
        return acc_num

    # --- TRANSACTIONS ---
    def deposit(self, account_number, amount):
        account = self.accounts.get(account_number)
        if not account or account.status != "Active":
            print("Error: Account not found or inactive.")
            return
        if amount <= 0:
            print("Error: Deposit amount must be positive.")
            return

        account.balance += amount
        self.transactions.append(Transaction(account_number, "Deposit", amount, account.balance))
        print(f"\nAccount: {account_number}\nAmount: GH₵{amount:,.2f}\nTransaction successful.")
        print(f"New Balance:\nGH₵{account.balance:,.2f}\n")

    def withdraw(self, account_number, amount):
        account = self.accounts.get(account_number)
        if not account or account.status != "Active":
            print("Error: Account not found or inactive.")
            return
        if amount > account.balance:
            print("Error: Insufficient funds.")
            return

        account.balance -= amount
        self.transactions.append(Transaction(account_number, "Withdrawal", -amount, account.balance))
        print(f"\nAccount: {account_number}\nAmount: GH₵{amount:,.2f}\nTransaction successful.")
        print(f"New Balance:\nGH₵{account.balance:,.2f}\n")

    def transfer(self, from_acc, to_acc, amount):
        source = self.accounts.get(from_acc)
        target = self.accounts.get(to_acc)

        if not source or source.status != "Active":
            print("Error: Source account not found or inactive.")
            return
        if not target or target.status != "Active":
            print("Error: Target account not found or inactive.")
            return
        if amount > source.balance:
            print("Error: Insufficient funds for transfer.")
            return

        source.balance -= amount
        target.balance += amount
        self.transactions.append(Transaction(from_acc, "Transfer", -amount, source.balance))
        self.transactions.append(Transaction(to_acc, "Transfer", amount, target.balance))

        print(f"\nFrom: {from_acc}\nTo:   {to_acc}\nAmount: GH₵{amount:,.2f}\nTransfer successful.")
        print(f"New Balance:\nGH₵{source.balance:,.2f}\n")

    # --- HISTORY & STATEMENTS ---
    def view_transaction_history(self, account_number):
        acc_txs = [tx for tx in self.transactions if tx.account_number == account_number]
        print("=" * 48)
        print("              TRANSACTION HISTORY")
        print("=" * 48)
        print(f"{'DATE':<14}{'TYPE':<14}{'AMOUNT':<13}{'BALANCE':<10}")
        print("-" * 48)
        for tx in acc_txs:
            sign = "+" if tx.amount > 0 else ""
            amt_str = f"{sign}{tx.amount:.2f}"
            print(f"{tx.date:<14}{tx.type:<14}{amt_str:<13}{tx.post_balance:.2f}")
        print("-" * 48 + "\n")

    def generate_statement(self, account_number):
        account = self.accounts.get(account_number)
        if not account:
            print("Account not found.")
            return
        customer = self.customers.get(account.customer_id)
        acc_txs = [tx for tx in self.transactions if tx.account_number == account_number]

        print("=" * 46)
        print("             ACCOUNT STATEMENT")
        print("=" * 46)
        print("Bank: Python Bank\n")
        print(f"Customer:\n{customer.full_name}\n")
        print(f"Account Number:\n{account.account_number}\n")
        print(f"Account Type:\n{account.account_type}\n")
        print("-" * 46)
        print(f"{'Transaction':<17}{'Date':<14}{'Amount':<15}")
        print("-" * 46)
        for tx in acc_txs:
            sign = "+" if tx.amount > 0 else ""
            amt_str = f"{sign}{tx.amount:,.0f}"
            print(f"{tx.type:<17}{tx.date:<14}{amt_str:<15}")
        print("-" * 46)
        print(f"\nCurrent Balance: GH₵{account.balance:,.0f}")
        print("=" * 46 + "\n")


# --- EXAMPLE RUN TIME DEMONSTRATION ---
if __name__ == "__main__":
    bank = BankManagementSystem()

    # 1. Create Customer & Accounts
    c_id = bank.create_customer("John Mensah", "15/05/1990", "+233200000000", "john@example.com", "Accra, Ghana")
    acc_1 = bank.create_account(c_id, "Savings", 2000.00)
    
    c_id2 = bank.create_customer("Sarah Addo", "20/02/1995", "+233500000000", "sarah@example.com", "Kumasi, Ghana")
    acc_2 = bank.create_account(c_id2, "Current", 500.00)

    # 2. Perform Transactions
    bank.deposit(acc_1, 500.00)
    bank.withdraw(acc_1, 200.00)
    bank.transfer(acc_1, acc_2, 300.00)

    # 3. View History and Generate Statement
    bank.view_transaction_history(acc_1)
    bank.generate_statement(acc_1)
