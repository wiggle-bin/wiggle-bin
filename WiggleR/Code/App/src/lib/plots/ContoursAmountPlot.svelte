<script lang="ts">
	import BasePlot from './BasePlot.svelte';
	import { start, end } from '../../store/store';
	import { getContoursData, convertDates } from './data.svelte';

	let startTime = '';
	let endTime = '';
	let trace: Plotly.Data;
	start.subscribe((time) => (startTime = time));
	end.subscribe((time) => (endTime = time));

    $: getTrace(startTime, endTime);

    async function getTrace(startTime: string, endTime: string) {
        const data = await getContoursData(startTime, endTime);
        trace = {
			x: convertDates(data.dates),
			y: data.contourAmount,
			type: 'scatter'
		}
    }
</script>

{#if trace}
	<BasePlot
		trace={trace}
		title="Contour Amount"
		xaxisTitle="Time in minutes"
		yaxisTitle="Amount of Contours"
	/>
{/if}
