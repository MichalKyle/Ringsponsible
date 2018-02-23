using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Runtime.CompilerServices;

using Xamarin.Forms;

using RingSponsible.Models;
using RingSponsible.Services;

namespace RingSponsible.ViewModels
{
    public class BaseViewModel : ObservableObject
    {
        public delegate void BluetoothRecData(object sender, BluetoothEventArgs e);

        public IDataStore<Item> DataStore => DependencyService.Get<IDataStore<Item>>() ?? new MockDataStore();
        public BluetoothBase bluCore = null;

        public BaseViewModel()
        {
            bluCore = new BluetoothBase(new BluetoothRecData(Message_Recieved));
        }

        bool isBusy = false;
        public bool IsBusy
        {
            get { return isBusy; }
            set { SetProperty(ref isBusy, value); }
        }

        string title = string.Empty;
        public string Title
        {
            get { return title; }
            set { SetProperty(ref title, value); }
        }

        #region Helper Functions
        protected bool SetProperty<T>(ref T backingStore, T value,
            [CallerMemberName]string propertyName = "",
            Action onChanged = null)
        {
            if (EqualityComparer<T>.Default.Equals(backingStore, value))
                return false;

            backingStore = value;
            onChanged?.Invoke();
            OnPropertyChanged(propertyName);
            return true;
        }
        #endregion

        BluetoothMessageTypes mess = BluetoothMessageTypes.NONE;
        private BluetoothMessageTypes LastMessageType
        {
            get { return mess; }
            set { SetProperty(ref mess, value); }
        }
        [DependsUpon("LastMessageType")]
        public string LastMessageTypeStr
        {
            get { return LastMessageType.ToString(); }
        }

        private void Message_Recieved(object sender, BluetoothEventArgs e)
        {
            LastMessageType = e.mess;
        }
    }
}
