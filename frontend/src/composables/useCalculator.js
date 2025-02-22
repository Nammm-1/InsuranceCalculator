import { ref, computed } from 'vue';
import { INSURANCE_RATE } from '../constants';

export function useCalculator() {
  const vehicleType = ref(null);
  const currency = ref(null);
  const exchangeRate = ref(1.0);
  const daysInsured = ref(null);
  const propertyDamageCoverage = ref(6000);
  const vehicleValue = ref(null);
  const calculations = ref([]);

  const multiplier = computed(() => {
    if (!daysInsured.value) return 0;
    return daysInsured.value / 365.0;
  });

  const calculatePremium = () => {
    const thirdPartyRate = vehicleType.value.rate;
    const additionalCoverage = (propertyDamageCoverage.value - 6000) * 0.02 / exchangeRate.value;
    const thirdPartyCoverage = (thirdPartyRate / exchangeRate.value) * multiplier.value;
    const premium = INSURANCE_RATE * vehicleValue.value * multiplier.value;
    const totalCost = (thirdPartyCoverage + premium + additionalCoverage) * exchangeRate.value;

    const calculation = {
      thirdPartyRate: thirdPartyCoverage * exchangeRate.value,
      premium: premium * exchangeRate.value,
      totalCost,
      currency: currency.value.value
    };

    calculations.value.push(calculation);
    return calculation;
  };

  const grandTotal = computed(() => {
    return calculations.value.reduce((sum, calc) => sum + calc.totalCost, 0);
  });

  const reset = () => {
    vehicleType.value = null;
    currency.value = null;
    exchangeRate.value = 1.0;
    daysInsured.value = null;
    propertyDamageCoverage.value = 6000;
    vehicleValue.value = null;
  };

  const resetAll = () => {
    reset();
    calculations.value = [];
  };

  return {
    vehicleType,
    currency,
    exchangeRate,
    daysInsured,
    propertyDamageCoverage,
    vehicleValue,
    calculations,
    grandTotal,
    calculatePremium,
    reset,
    resetAll
  };
}
